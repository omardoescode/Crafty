#include "event_dispatcher.h"
#include <mutex>

namespace common {
EventDispatcher& EventDispatcher::instance() {
  static EventDispatcher instance;
  return instance;
}

void EventDispatcher::clear_all() {
  std::unique_lock<std::shared_mutex> lock(_mtx);
  _handlers.clear();
}
}  // namespace common
