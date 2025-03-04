#include "BlockCategoryPanel.h"
#include <imgui.h>
#include <iostream>
#include "block/BlockLibrary.h"

namespace ui {
BlockCategoryPanel::BlockCategoryPanel(UIOptions& options)
    : _options(options) {}

void BlockCategoryPanel::draw() {
  auto& lib = model::BlockLibrary::instance();
  auto categories = lib.categories();

  ImGui::BeginChild("BlockCategoryPanel", ImVec2(0, 0),
                    ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_Border, true);
  for (auto& name : categories) {
    if (ImGui::Button(name.c_str())) _options.set_current_category(name);
  }
  ImGui::EndChild();
}
}  // namespace ui
