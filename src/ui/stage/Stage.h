#pragma once
#include <SDL3/SDL.h>
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
};
}  // namespace ui
