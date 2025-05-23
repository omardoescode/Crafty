#pragma once
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <typeindex>
#include <vector>
#include "common_logger.h"
#include "event_base.h"

namespace common {
class EventDispatcher {
public:
  class Token;
  /**
   * @brief Type alias for the handler id
   */
  typedef unsigned int HandlerId;

  /**
   * @brief Type alias for the subscription token pointer
   */
  typedef std::unique_ptr<Token> TokenP;

public:
  /**
   * @brief A Subscription Token
   *
   * @note when destroyed, it unsubscribes from the event dispatcher
   */
  class Token {
  public:
    Token(EventDispatcher &dispatcher, std::type_index idx,
          EventDispatcher::HandlerId id);
    ~Token();

    /**
     * @brief Prevent copying of subscriptions
     */
    Token(const Token &) = delete;
    Token &operator=(const Token &) = delete;

    /**
     * @brief Unsubscribe from the token's handler
     */
    void cancel();

  private:
    bool _active;
    EventDispatcher::HandlerId _id;
    std::type_index _type_id;
    EventDispatcher &_dispatcher;  // dependency injection
  };

public:
  EventDispatcher(EventDispatcher &) = delete;
  EventDispatcher &operator=(EventDispatcher &) = delete;

  /**
   * @brief Get the singleton instance
   *
   * @return Return a reference to the singleton instance of EventDispatcher
   */
  static EventDispatcher &instance();

  /**
   * @brief Subscribe to an event
   * @param EventType The type of event to subscribe to
   * @param handler Function to call when the event is published
   * @param A subsciption token pointer that can be used to cancel a
   * subscription
   */
  template <typename EventType>
  [[nodiscard]]
  TokenP subscribe(std::function<void(std::shared_ptr<EventType>)> handler_func,
                   std::string &&debug = "") {
    static_assert(std::is_base_of<EventBase, EventType>::value,
                  "Event type must inherit from EventBase");

    // Get the type index
    std::type_index event_type(typeid(EventType));

    // Create a new id
    auto id = _next_id.fetch_add(1);

    // Check for debug tag
    static std::atomic _debug_counter = 0;
    if (debug.empty())
      debug = "debug_" + std::to_string(_debug_counter.fetch_add(1));

    // Initiate the lock
    std::unique_lock<std::shared_mutex> lock(_mtx);

    Handler handler;
    handler.id = id;
    handler.func = [handler_func](std::shared_ptr<EventBase> evt) {
      if (auto downcast = std::dynamic_pointer_cast<EventType>(evt))
        handler_func(downcast);
    };
    handler.debug_tag = std::move(debug);

    _handlers[event_type].emplace_back(std::move(handler));

    _count.fetch_add(1);

    common_logger().info("Handler {} has been created", debug);
    return std::make_unique<Token>(instance(), event_type, id);
  }

  /**
   * @brief Publish an event
   * @param EventType The type of event to publish
   * @param evt The event to publish
   */
  template <typename EventType>
  void publish(std::shared_ptr<EventType> evt) {
    static_assert(std::is_base_of<EventBase, EventType>::value,
                  "Event type must inherit from EventBase");

    std::type_index event_type(typeid(EventType));

    // minimize the lock type by copying instead
    std::vector<Handler> handlers_copy;
    {
      std::shared_lock lck(_mtx);
      auto it = _handlers.find(event_type);
      if (it != _handlers.end()) {
        handlers_copy = it->second;
      }
    }

    for (auto &handler : handlers_copy) {
      try {
        handler.func(evt);
      } catch (const std::exception &) {
        throw common_logger().error("Failed to run handler");
      }
    }
  }

  /**
   * @brief Clear all the handlers associated with an event
   * @param EventType The event type to clear
   */
  template <typename EventType>
  void clear() {
    std::unique_lock<std::shared_mutex> lock(_mtx);
    std::type_index event_type(typeid(EventType));
    auto it = _handlers.find(event_type);
    if (it != _handlers.end()) _handlers.erase(it);
  }

  void clear_all();

private:
  struct Handler {
    HandlerId id;
    std::string debug_tag;
    std::function<void(std::shared_ptr<EventBase>)> func;
  };

private:
  EventDispatcher();
  void unsubscribe(std::type_index event_type, HandlerId target_id);
  Token generate_token(std::type_index event_type, HandlerId target_id);

private:
  std::unordered_map<std::type_index, std::vector<Handler>> _handlers;
  std::shared_mutex _mtx;
  std::atomic<HandlerId> _next_id;
  std::atomic_int _count;

  friend class Token;
};
}  // namespace common
