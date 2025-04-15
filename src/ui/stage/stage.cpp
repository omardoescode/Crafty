#include "stage.h"
#include <SDL3/SDL_render.h>
#include "events/event_dispatcher.h"
#include "imgui.h"
#include "model_events.h"
#include "ui_logger.h"

static auto& dispatcher = common::EventDispatcher::instance();
namespace ui {
Stage::Stage(UIOptions& options, SDL_Renderer* renderer) : _options(options) {
  on_character_created_token =
      dispatcher.subscribe<model::events::onCharacterCreated>(
          [this](std::shared_ptr<model::events::onCharacterCreated> evt) {
            auto& chr = evt->character;
            _characters_views.emplace(
                chr->id(), std::make_shared<CharacterView>(_options, chr));

            ui_logger().info("Added a new character for {} on stage",
                             chr->name());
          });
  before_character_deleted_token =
      dispatcher.subscribe<model::events::beforeCharacterDeleted>(
          [this](std::shared_ptr<model::events::beforeCharacterDeleted> evt) {
            auto& chr = evt->character;
            auto itr = _characters_views.find(chr->id());
            assert(itr != _characters_views.end());
            _characters_views.erase(itr);
            ui_logger().info("Removing character {} from stage", chr->name());
          });
}

void Stage::draw() {
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  // Get position AFTER accounting for padding (cursor position)
  ImVec2 p_min = ImGui::GetCursorScreenPos();

  // Get available space in current child window
  ImVec2 size = ImGui::GetContentRegionAvail();

  ImVec2 p_max(p_min.x + size.x, p_min.y + size.y);

  draw_list->AddRectFilled(p_min, p_max, IM_COL32(255, 255, 255, 255),
                           _options.rounding);

  // Draw The characters
  for (auto& [id, chr_vw] : _characters_views) {
    chr_vw->draw();
  }
}
}  // namespace ui
