#include "CharacterManager.h"
#include <imgui.h>
#include <cassert>
#include <iostream>
#include <thread>
#include "project_manager.h"
#include "utils/MaterialSymbols.h"
#include "utils/file_utils.h"
#include "utils/images.h"

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
      auto default_path = _options.executable_path() / "builtin";
      assert(std::filesystem::exists(default_path) &&
             "Default path for uploading characters doesn't exist");
      auto path = upload_file("Image Files", "png,jpg,jpeg,gif,bmp,webppng",
                              default_path.c_str());
      if (!path.empty()) {
        upload_char(path);
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

void CharacterManager::upload_char(std::filesystem::path path) {
  auto asset_folder = _options.asset_dest_folder();
  std::thread([this, path, asset_folder]() {
    auto& mgr = model::ProjectManager::instance();
    auto new_char = mgr.add_character(path, asset_folder);
    // TODO: Create a character view
  }).detach();
}

}  // namespace ui
