#include "stage.h"
#include <SDL3/SDL_render.h>
#include <iostream>
#include "SDL3/SDL_opengl.h"
#include "events/event_dispatcher.h"
#include "events/events.h"
#include "imgui.h"

namespace ui {
Stage::Stage(UIOptions& options, SDL_Renderer* renderer)
    : _options(options), _sdlRenderer(renderer) {
  assert(renderer && "nullptr to renderer");
  // Create render target texture
  _gameTexture =
      SDL_CreateTexture(_sdlRenderer, SDL_PIXELFORMAT_RGBA32,
                        SDL_TEXTUREACCESS_TARGET, _options.RIGHT_SIDEBAR_WIDTH,
                        _options.RIGHT_SIDEBAR_WIDTH / _options.STAGE_ASPECT);

  assert(_gameTexture && "Failed to create game texture");

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

Stage::~Stage() {
  if (_gameTexture) SDL_DestroyTexture(_gameTexture);
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
}
}  // namespace ui
