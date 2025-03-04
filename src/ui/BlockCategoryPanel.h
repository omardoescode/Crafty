#pragma once

#include <imgui.h>
#include <map>
#include <string>
#include "ui_options.h"
namespace ui {
class BlockCategoryPanel {
public:
public:
  BlockCategoryPanel(UIOptions& options);
  void draw();

private:
  UIOptions& _options;
};
}  // namespace ui
