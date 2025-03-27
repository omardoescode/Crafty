#include "CharacterManager.h"
#include <imgui.h>
#include <cassert>
#include <experimental/filesystem>
#include <iostream>
#include <thread>
#include "utils/MaterialSymbols.h"
#include "utils/images.h"
#include "utils/platform.h"
#include "utils/upload_file.h"

namespace ui {
CharacterManager::CharacterManager(UIOptions& options) : _options(options) {}
void CharacterManager::draw() {
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 7.0f);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(40, 40, 40, 255));

  if (ImGui::BeginChild("RoundedContainer", ImVec2(0, 0), true)) {
    ImGui::BeginGroup();

    ImGui::AlignTextToFramePadding();
    ImGui::Text("New Sprite: ");
    ImGui::SameLine();

    ImGui::PushFont(_options.get_font(_options.ICONS_FONT_MEDIUM));
    if (ImGui::Button(ICON_MD_ADD)) {
      auto default_path = get_executable_path().parent_path() / "builtin";
      assert(std::filesystem::exists(default_path) &&
             "Default path for uploading characters doesn't exist");
      auto path = upload_file("Image Files", "png,jpg,jpeg,gif,bmp,webppng",
                              default_path.c_str());
      if (!path.empty()) {
        upload_char();
      }
    }
    ImGui::PopFont();
    ImGui::EndGroup();
    ImGui::Separator();
  }
  ImGui::EndChild();

  // Cleanup styles
  ImGui::PopStyleColor();
  ImGui::PopStyleVar();
}

void CharacterManager::upload_char() {
  std::thread([]() {
    // TODO: Upload Character
  }).detach();
}

}  // namespace ui
