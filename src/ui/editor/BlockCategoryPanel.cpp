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

  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.0f);
  ImGui::BeginChild("BlockCategoryPanel", ImVec2(0, 0),
                    ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                        ImGuiChildFlags_Border);
  if (ImGui::BeginTable("table", 2)) {
    for (auto& name : categories) {
      ImGui::TableNextColumn();
      ImGui::Button(name.c_str(), ImVec2(80, 0));
    }
    ImGui::EndTable();
  }
  ImGui::PopStyleVar();
  ImGui::EndChild();
}
}  // namespace ui
