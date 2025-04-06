#pragma once
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <typeindex>
#include <vector>
#include "event_base.h"

namespace common {
class EventDispatcher {
public:
  typedef size_t handler_id;

public:
  EventDispatcher(EventDispatcher&) = delete;
  EventDispatcher& operator=(EventDispatcher&) = delete;

  static EventDispatcher& instance();
  template <typename EventType>
  handler_id subscribe(
      std::function<void(std::shared_ptr<EventType>)> handler) {
    static_assert(std::is_base_of<EventBase, EventType>::value,
                  "Event type must inherit from EventBase");

    // Get the type index
    std::type_index event_type(typeid(EventType));

    // Create a new id
    auto id = _next_id.fetch_add(1);

    std::unique_lock<std::shared_mutex> lock(_mtx);

    _handlers[event_type].emplace_back(
        Handler{id, [handler](std::shared_ptr<EventBase> evt) {
                  if (auto downcast = std::dynamic_pointer_cast<EventType>(evt))
                    handler(downcast);
                }});
    return id;
  }

  template <typename EventType>
  void publish(std::shared_ptr<EventType> evt) {
    static_assert(std::is_base_of<EventBase, EventType>::value,
                  "Event type must inherit from EventBase");

    std::type_index event_type(typeid(EventType));
    std::vector<Handler> handlers;

    {
      std::shared_lock lck(_mtx);
      auto it = _handlers.find(event_type);
      if (it != _handlers.end()) {
        handlers = it->second;
      }
    }

    for (auto& handler : handlers) {
      handler.func(evt);
    }
  }

  template <typename EventType>
  void clear() {
    std::unique_lock<std::shared_mutex> lock(_mtx);
    std::type_index event_type(typeid(EventType));
    auto it = _handlers.find(event_type);
    if (it != _handlers.end()) _handlers.erase(it);
  }

  template <typename EventType>
  void unsubscribe(handler_id target_id) {
    std::unique_lock<std::shared_mutex> lock(_mtx);
    std::type_index event_type(typeid(EventType));
    auto evt_iterator = _handlers.find(event_type);
    if (evt_iterator == _handlers.end()) return;

    auto handler_it = std::ranges::find_if(
        evt_iterator->second.begin(), evt_iterator->second.end(),
        [target_id](auto& handler) { return handler.id == target_id; });

    if (handler_it == evt_iterator->second.end()) return;

    evt_iterator->second.erase(handler_it);
  }

  void clear_all();

private:
  struct Handler {
    handler_id id;
    std::function<void(std::shared_ptr<EventBase>)> func;
  };

private:
  EventDispatcher() = default;

private:
  std::unordered_map<std::type_index, std::vector<Handler>> _handlers;
  std::shared_mutex _mtx;
  std::atomic<handler_id> _next_id;
};
}  // namespace common
