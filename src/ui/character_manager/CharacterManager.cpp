#include "CharacterManager.h"
#include <imgui.h>
#include <cassert>
#include "utils/MaterialSymbols.h"
#include "utils/images.h"

namespace ui {
CharacterManager::CharacterManager(UIOptions& options) : _options(options) {}
void CharacterManager::draw() {
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 7.0f);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(40, 40, 40, 255));

  if (ImGui::BeginChild("RoundedContainer", ImVec2(0, 0), true)) {
    ImGui::Columns(2, "##SplitColumns", false);
    ImGui::SetColumnWidth(0, LEFT_SECTION_WIDTH);

    // Left Section
    ImGui::BeginChild("FixedSection");
    draw_button();
    ImGui::EndChild();

    // Draw Separator
    ImGui::NextColumn();

    // Right Section
    ImGui::BeginChild("StretchedSection");
    ImGui::Text("Stretched Content");
    ImGui::EndChild();
  }
  ImGui::EndChild();

  // Cleanup styles
  ImGui::PopStyleColor();
  ImGui::PopStyleVar();
}

void CharacterManager::draw_button() {
  ImGui::PushFont(_options.get_font(_options.ICONS_FONT_MEDIUM));
  ImGui::Button(ICON_MD_HOME, ImVec2(ImGui::GetContentRegionAvail().x, 0));
  ImGui::PopFont();
}
}  // namespace ui
