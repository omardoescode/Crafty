#include "block_picker.h"
#include "block/block_library.h"
#include "imgui.h"
#include "ui_logger.h"
#include "ui_options.h"

namespace ui {
BlockPicker::BlockPicker(UIOptions& options) : _options(options) {}

void BlockPicker::fetch_category_instances(const std::string& category_name) {
  auto& lib = model::BlockLibrary::instance();
  auto blocks_definitions = lib.category_blocks(category_name);

  for (auto& block_def : blocks_definitions) {
    BlockView view(_options, lib.create_dummy_instance(block_def->id()));
    _views_per_category[category_name].push_back(view);
  }
}

void BlockPicker::draw() {
  auto& cur_category = _options.current_category();
  // Ensure we have the category instances
  if (_views_per_category.count(cur_category) == 0) {
    fetch_category_instances(cur_category);
  }

  // Draw
  auto& views = _views_per_category[cur_category];
  ImGui::Columns(1);
  for (int row = 0; row < views.size(); row++) {
    ImGui::PushID(row);
    views[row].draw();
    ImGui::PopID();
    ImGui::NextColumn();
  }
}
}  // namespace ui
