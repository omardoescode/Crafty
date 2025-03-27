#include "action_deferrer.h"
#include <iostream>
#include <mutex>

namespace ui {
ActionDeferrer& ActionDeferrer::instance() {
  static ActionDeferrer instance;
  return instance;
}

void ActionDeferrer::defer(std::function<void()> func, int priority,
                           const char* debug_tag) {
  std::lock_guard lock(_actions_mtx);
  _actions.emplace(
      Action{.func = func, .priortiy = priority, .debug_tag = debug_tag});
}

bool ActionDeferrer::Action::operator<(const Action& rhs) const {
  return priortiy < rhs.priortiy;
}

void ActionDeferrer::flush() {
  std::priority_queue<Action> swap_actions;
  {
    std::lock_guard lck(_actions_mtx);
    _actions.swap(swap_actions);
  }

  while (!swap_actions.empty()) {
    auto act = swap_actions.top();
    swap_actions.pop();

    try {
      act.func();
    } catch (...) {
      std::cerr << "Failed action: " << act.debug_tag << std::endl;
    }
  }
}
}  // namespace ui
