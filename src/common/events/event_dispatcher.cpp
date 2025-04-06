#include "event_dispatcher.h"
#include <mutex>

static auto &dispatcher = common::EventDispatcher::instance();
namespace common {
EventDispatcher &EventDispatcher::instance() {
  static EventDispatcher instance;
  return instance;
}

void EventDispatcher::clear_all() {
  std::unique_lock<std::shared_mutex> lock(_mtx);
  _handlers.clear();
}

void EventDispatcher::unsubscribe(std::type_index event_type,
                                  handler_id target_id) {
  std::unique_lock<std::shared_mutex> lock(_mtx);
  auto evt_iterator = _handlers.find(event_type);
  if (evt_iterator == _handlers.end()) return;

  auto handler_it = std::ranges::find_if(
      evt_iterator->second.begin(), evt_iterator->second.end(),
      [target_id](auto &handler) { return handler.id == target_id; });

  if (handler_it == evt_iterator->second.end()) return;

  evt_iterator->second.erase(handler_it);
}

EventDispatcher::Token::Token(std::type_index idx,
                              EventDispatcher::handler_id id)
    : _type_id(idx), _id(id), _active(true) {}
EventDispatcher::Token::Token(Token &&rhs)
    : _id(std::move(rhs._id)),
      _type_id(std::move(rhs._type_id)),
      _active(rhs._active) {
  rhs._active = false;
}

EventDispatcher::Token &EventDispatcher::Token::operator=(Token &&rhs) {
  if (this != &rhs) {
    unsubscribe();  // if there'are any existing ones before
    _id = std::move(rhs._id);
    _type_id = std::move(rhs._type_id);
    _active = rhs._active;
    rhs._active = false;
  }
  return *this;
}

void EventDispatcher::Token::unsubscribe() {
  if (_active) {
    dispatcher.unsubscribe(_type_id, _id);
    _active = false;
  }
}
}  // namespace common
