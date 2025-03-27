#pragma once

#include <functional>
#include <mutex>
#include <queue>
namespace ui {
/**
 * @breif Delays an action until the current frame is finished and before the
 * next frame
 */
class ActionDeferrer {
public:
  // remove copying
  ActionDeferrer(ActionDeferrer&) = delete;
  ActionDeferrer& operator=(ActionDeferrer&) = delete;

  static ActionDeferrer& instance();

  void defer(std::function<void()>, int priority = 0,
           const char* debug_tag = nullptr);
  void flush();

private:
  ActionDeferrer() = default;

private:
  struct Action {
    std::function<void()> func;
    int priortiy;
    const char* debug_tag;

    bool operator<(const Action& rhs) const;
  };
  std::priority_queue<Action> _actions;
  std::mutex _actions_mtx;
};
}  // namespace ui
