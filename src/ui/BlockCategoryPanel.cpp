#include "BlockCategoryPanel.h"
#include <imgui.h>

namespace ui {
BlockCategoryPanel::BlockCategoryPanel(UIOptions& options) : _options(options) {
  categories = {{"Motion", {ImVec4(255, 0, 0, 1.0f), ImVec4(255, 0, 0, 0.5f)}}};
}

void BlockCategoryPanel::draw() {
  ImGui::BeginChild("BlockCategoryPanel", ImVec2(0, 0),
                    ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_Border, true);
  for (auto& [name, info] : categories) {
    ImGui::PushStyleColor(ImGuiCol_Button, info.color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, info.color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, info.hover_color);
    if (ImGui::Button(name)) _options.set_current_category(name);
    ImGui::PopStyleColor(3);
  }
  ImGui::EndChild();
}
}  // namespace ui
