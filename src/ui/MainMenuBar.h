#pragma once

#include "imgui.h"
#include "ui_options.h"
namespace ui {
class MainMenuBar {
public:
  MainMenuBar(UIOptions& options);
  void draw();

private:
  UIOptions& _options;
};
}  // namespace ui
