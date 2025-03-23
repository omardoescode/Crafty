#pragma once
#include <SDL3/SDL.h>
#include "imgui.h"
#include "ui_options.h"

namespace ui {
class Stage {
public:
  Stage(UIOptions& options, SDL_Renderer* renderer);
  ~Stage();
  void draw();

private:
  UIOptions& _options;
  SDL_Renderer* _sdlRenderer;
  SDL_Texture* _gameTexture;
  float _texWidth = 800;  // Initial texture size
  float _texHeight = 600;
};
}  // namespace ui
