#pragma once

#include "SDL3/SDL_opengl.h"
#include "character.h"
#include "ui_options.h"
namespace ui {
class CharacterMiniView {
public:
  CharacterMiniView(UIOptions&, std::shared_ptr<model::Character>);
  ~CharacterMiniView();
  void draw();

private:
  constexpr static float sz_x = 70;
  constexpr static float sz_y = 70;

private:
  /**
   * Load the texture
   *
   * NOTE: This must be called in a non-detached thread
   */
  void load_texture_once();

private:
  UIOptions& _options;
  std::shared_ptr<model::Character> _character;
  GLuint out_texture;
  int out_width;
  int out_height;
};
}  // namespace ui
