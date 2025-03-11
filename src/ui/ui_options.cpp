#include "ui_options.h"
namespace ui {
UIOptions::UIOptions() : _running{true} {}
const bool& UIOptions::running() const { return _running; }
void UIOptions::close() { _running = false; }

const std::string UIOptions::current_category() const {
  return _current_cateogry;
}
void UIOptions::set_current_category(const std::string& value) {
  _current_cateogry = value;
}
}  // namespace ui
