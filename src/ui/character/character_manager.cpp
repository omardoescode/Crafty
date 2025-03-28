#include "character_manager.h"
#include <imgui.h>
#include <cassert>
#include <cmath>
#include <memory>
#include "character/character_miniview.h"
#include "character/character_view.h"
#include "events/event_dispatcher.h"
#include "events/events.h"
#include "project_manager.h"
#include "utils/file_utils.h"
#include "utils/images.h"
#include "utils/material_symbols.h"

namespace ui {
CharacterManager::CharacterManager(UIOptions& options) : _options(options) {
  auto& dispatcher = common::EventDispatcher::instance();
  dispatcher.subscribe<model::events::onCharacterCreated>(
      [this](std::shared_ptr<model::events::onCharacterCreated> evt) {
        auto& chr = evt->character;
        _miniviews.emplace(chr->id(),
                           std::make_shared<CharacterMiniView>(_options, chr));
      });

  dispatcher.subscribe<model::events::beforeCharacterDeleted>(
      [this](std::shared_ptr<model::events::beforeCharacterDeleted> evt) {
        auto& chr = evt->character;
        auto itr = _miniviews.find(chr->id());
        assert(itr != _miniviews.end());
        _miniviews.erase(itr);
      });

  upload_char(std::filesystem::path("./builtin/cat.png"));
}
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
      handle_add_click();
    }
    ImGui::PopFont();
    ImGui::EndGroup();
    ImGui::Separator();

    ImGui::Columns(
        ImGui::GetContentRegionAvail().x / CharacterMiniView::CARD_WIDTH, NULL,
        false);
    for (auto& [id, mv] : _miniviews) {
      mv->draw();
      ImGui::NextColumn();
    }
  }
  ImGui::EndChild();

  // Cleanup styles
  ImGui::PopStyleColor();
  ImGui::PopStyleVar();
}

void CharacterManager::handle_add_click() {
  auto default_path = _options.executable_path() / "builtin";
  assert(std::filesystem::exists(default_path) &&
         "Default path for uploading characters doesn't exist");
  auto path = upload_file("Image Files", "png,jpg,jpeg,gif,bmp,webppng",
                          default_path.c_str());

  if (!path.empty()) {
    upload_char(path);
  }
}

void CharacterManager::upload_char(std::filesystem::path path) {
  auto asset_folder = _options.asset_dest_folder();
  auto& mgr = model::ProjectManager::instance();
  auto new_char = mgr.add_character(path, asset_folder);
  _options.set_current_character(new_char);
}

}  // namespace ui
