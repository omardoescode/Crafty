#include "stage.h"
#include <SDL3/SDL_render.h>
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
  if (!_gameTexture) return;

  // 1. Render game to texture
  SDL_SetRenderTarget(_sdlRenderer, _gameTexture);
  SDL_SetRenderDrawColor(_sdlRenderer, 0, 0, 0, 1);
  SDL_RenderClear(_sdlRenderer);

  // Your game rendering here...

  // 2. Reset render target
  SDL_SetRenderTarget(_sdlRenderer, nullptr);

  // 3. Display in ImGui
  if (ImGui::BeginChild("Game View", ImVec2(_options.RIGHT_SIDEBAR_WIDTH,
                                            _options.RIGHT_SIDEBAR_WIDTH /
                                                _options.STAGE_ASPECT))) {
    // Get texture properties
    SDL_PropertiesID props = SDL_GetTextureProperties(_gameTexture);

    // Get OpenGL texture ID using SDL3 property string
    const char* OPENGL_TEXTURE_PROP = "sdl.texture.opengl.texture";
    GLuint glTextureID = static_cast<GLuint>(
        SDL_GetNumberProperty(props, OPENGL_TEXTURE_PROP, 0));

    // Get texture size
    float texWidth, texHeight;
    SDL_GetTextureSize(_gameTexture, &texWidth, &texHeight);

    // Calculate display size
    ImVec2 availSize = ImGui::GetContentRegionAvail();
    float aspect = static_cast<float>(texWidth) / static_cast<float>(texHeight);
    ImVec2 displaySize(availSize.x, availSize.x / aspect);

    if (displaySize.y > availSize.y) {
      displaySize.y = availSize.y;
      displaySize.x = displaySize.y * aspect;
    }

    // Display the texture
    ImGui::Image((intptr_t)glTextureID, displaySize,
                 ImVec2(0, 1),  // UV coordinates
                 ImVec2(1, 0));
    ImGui::EndChild();
  }
}
}  // namespace ui
