#include "BlockCategoryPanel.h"
#include <imgui.h>
#include "block/block_library.h"

namespace ui {
BlockCategoryPanel::BlockCategoryPanel(UIOptions& options)
    : _options(options) {}

void BlockCategoryPanel::draw() {
  auto& lib = model::BlockLibrary::instance();
  auto categories = lib.categories();

  if (ImGui::BeginTable("table", 2)) {
    for (auto& name : categories) {
      ImGui::TableNextColumn();
      if (ImGui::Button(name.c_str(), ImVec2(80, 0))) {
        _options.set_current_category(name);
      }
    }
    ImGui::EndTable();
  }
}
}  // namespace ui
