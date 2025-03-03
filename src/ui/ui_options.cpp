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
ImVec4 UIOptions::get_hover_color(ImVec4 color) {
  color.z -= 0.6f;
  return color;
}
}  // namespace ui
