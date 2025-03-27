#pragma once

#include "SDL3/SDL_opengl.h"
#include "character.h"
#include "ui_options.h"
namespace ui {
class CharacterMiniView {
public:
  constexpr static float CARD_WIDTH = 100.0f;
  constexpr static float CARD_HEIGHT = 130.0f;
  constexpr static float IMAGE_X = 80.f;
  constexpr static float IMAGE_Y = 80.f;

public:
  CharacterMiniView(UIOptions&, std::shared_ptr<model::Character>);
  ~CharacterMiniView();
  void draw();

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
