#pragma once

#include <imgui.h>
#include <map>
#include <string>
#include "ui_options.h"
namespace ui {
class BlockCategoryPanel {
public:
  // TODO: Move to model layer
  struct Category {
    ImVec4 color;
    ImVec4 hover_color;
  };

public:
  BlockCategoryPanel(UIOptions& options);
  void draw();

private:
  UIOptions& _options;
  std::map<const char*, Category> categories;  // hardcodes for now
};
}  // namespace ui
