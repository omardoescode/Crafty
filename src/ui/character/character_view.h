#pragma once

#include <memory>
#include "SDL3/SDL_render.h"
#include "character.h"
#include "ui_options.h"
namespace ui {
class CharacterView {
public:
  CharacterView(UIOptions& options, std::shared_ptr<model::Character>);
  void render(SDL_Renderer*);

private:
  std::shared_ptr<model::Character> _character;
  UIOptions& _options;
};
}  // namespace ui
