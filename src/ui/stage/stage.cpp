#include "stage.h"
#include <SDL3/SDL_render.h>
#include "events/event_dispatcher.h"
#include "events/events.h"
#include "imgui.h"

namespace ui {
Stage::Stage(UIOptions& options, SDL_Renderer* renderer) : _options(options) {
  auto& dispatcher = common::EventDispatcher::instance();
  dispatcher.subscribe<model::events::onCharacterCreated>(
      [this](std::shared_ptr<model::events::onCharacterCreated> evt) {
        auto& chr = evt->character;
        _characters_views.emplace(
            chr->id(), std::make_shared<CharacterView>(_options, chr));
      });
  dispatcher.subscribe<model::events::beforeCharacterDeleted>(
      [this](std::shared_ptr<model::events::beforeCharacterDeleted> evt) {
        auto& chr = evt->character;
        auto itr = _characters_views.find(chr->id());
        assert(itr != _characters_views.end());
        _characters_views.erase(itr);
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

  _options.set_stage_width(_options.RIGHT_SIDEBAR_WIDTH);
  // Draw The characters
  for (auto& [id, chr_vw] : _characters_views) {
    chr_vw->draw();
  }
}
}  // namespace ui
