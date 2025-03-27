#pragma once

#include <map>
#include <vector>
#include "block_view.h"
#include "ui_options.h"

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
