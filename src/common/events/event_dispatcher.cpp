#include "event_dispatcher.h"
#include <mutex>
#include "common_logger.h"

namespace common {
EventDispatcher::EventDispatcher() : _next_id(0) {}
EventDispatcher &EventDispatcher::instance() {
  static EventDispatcher instance;
  return instance;
}

void EventDispatcher::clear_all() {
  std::unique_lock<std::shared_mutex> lock(_mtx);
  _handlers.clear();
}

void EventDispatcher::unsubscribe(std::type_index event_type,
                                  HandlerId target_id) {
  std::unique_lock<std::shared_mutex> lock(_mtx);
  auto evt_iterator = _handlers.find(event_type);
  if (evt_iterator == _handlers.end()) return;

  auto handler_it = std::ranges::find_if(
      evt_iterator->second.begin(), evt_iterator->second.end(),
      [target_id](auto &handler) { return handler.id == target_id; });

  if (handler_it == evt_iterator->second.end()) return;

  evt_iterator->second.erase(handler_it);

  common_logger().info("Handler has been removed");
}

EventDispatcher::Token::Token(EventDispatcher &dispatcher, std::type_index idx,
                              EventDispatcher::HandlerId id)
    : _type_id(idx), _id(id), _active(true), _dispatcher(dispatcher) {}
EventDispatcher::Token::~Token() { cancel(); }

void EventDispatcher::Token::cancel() {
  if (_active) {
    _dispatcher.unsubscribe(_type_id, _id);
    _active = false;
  }
}

}  // namespace common
