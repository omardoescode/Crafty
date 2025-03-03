#include "ui_options.h"
namespace ui {
UIOptions::UIOptions() : _running{true} {}
const UIOptions::Option& UIOptions::running() const { return _running; }
void UIOptions::close() { _running = false; }
}  // namespace ui
