#pragma once

#include <map>
#include <vector>
#include "ui_options.h"
#include "views/BlockView.h"

namespace ui {
class BlockPicker {
public:
  BlockPicker(UIOptions& options);
  void draw();

private:
  void fetch_category_instances(const std::string& category);

private:
  UIOptions& _options;
  std::map<std::string, std::vector<BlockView>> _views_per_category;
};
}  // namespace ui
