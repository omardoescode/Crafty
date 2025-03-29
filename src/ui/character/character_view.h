#pragma once

#include <memory>
#include "SDL3/SDL_opengl.h"
#include "character.h"
#include "ui_options.h"
namespace ui {
class CharacterView {
public:
  CharacterView(UIOptions& options, std::shared_ptr<model::Character>);
  void draw();

private:
  void load_texture_once();

private:
  std::shared_ptr<model::Character> _character;
  UIOptions& _options;
  GLuint out_texture;
  int out_width;
  int out_height;
  float width, height;
};
}  // namespace ui
