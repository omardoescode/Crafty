#include "block_category_panel.h"
#include <imgui.h>
#include "block/block_library.h"
#include "project_manager.h"

namespace ui {
BlockCategoryPanel::BlockCategoryPanel(UIOptions& options)
    : _options(options) {}

void BlockCategoryPanel::draw() {
  auto lib = model::ProjectManager::instance().block_lib();
  auto categories = lib->categories();

  if (ImGui::BeginTable("table", 2)) {
    for (auto& name : categories) {
      ImGui::TableNextColumn();
      if (ImGui::Button(name.c_str(),
                        ImVec2(_options.LEFT_SIDEBAR_WIDTH / 2, 0))) {
        _options.set_current_category(name);
      }
    }
    ImGui::EndTable();
  }
}
}  // namespace ui
