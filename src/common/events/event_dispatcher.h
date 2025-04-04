#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <typeindex>
#include <vector>
namespace common {

class EventBase {
public:
  virtual ~EventBase() = default;
};

// TODO: When an element is removed, and a function has `this` in the closure,
// will this make a problem? Figure out and change this dependingly
class EventDispatcher {
public:
  EventDispatcher(EventDispatcher&) = delete;
  EventDispatcher& operator=(EventDispatcher&) = delete;

  static EventDispatcher& instance();

  template <typename EventType>
  void subscribe(std::function<void(std::shared_ptr<EventType>)> handler) {
    static_assert(std::is_base_of<EventBase, EventType>::value,
                  "Event type must inherit from EventBase");

    // Get the type index
    std::type_index event_type(typeid(EventType));

    std::unique_lock<std::shared_mutex> lock(_mtx);

    _handlers[event_type].push_back([handler](std::shared_ptr<EventBase> evt) {
      if (auto downcast = std::dynamic_pointer_cast<EventType>(evt))
        handler(downcast);
    });
  }

  template <typename EventType>
  void publish(std::shared_ptr<EventType> evt) {
    static_assert(std::is_base_of<EventBase, EventType>::value,
                  "Event type must inherit from EventBase");

    std::type_index event_type(typeid(EventType));
    std::vector<std::function<void(std::shared_ptr<EventBase>)>> handlers;

    {
      std::shared_lock lck(_mtx);
      auto it = _handlers.find(event_type);
      if (it != _handlers.end()) {
        handlers = it->second;
      }
    }

    for (auto& handler : handlers) {
      handler(evt);
    }
  }

  template <typename EventType>
  void clear() {
    std::unique_lock<std::shared_mutex> lock(_mtx);
    std::type_index event_type(typeid(EventType));
    auto it = _handlers.find(event_type);
    if (it != _handlers.end()) _handlers.erase(it);
  }

  void clear_all();

private:
  EventDispatcher() = default;

private:
  std::unordered_map<
      std::type_index,
      std::vector<std::function<void(std::shared_ptr<EventBase>)>>>
      _handlers;
  mutable std::shared_mutex _mtx;
};
}  // namespace common
