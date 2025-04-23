#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include "character/character_view.h"
#include "events/event_dispatcher.h"
#include "ui_options.h"

namespace ui {
class Stage {
public:
  Stage(UIOptions& options, SDL_Renderer* renderer);
  void draw();

private:
  UIOptions& _options;
  common::EventDispatcher::TokenP on_character_created_token;
  common::EventDispatcher::TokenP before_character_deleted_token;

  // Model
  std::unordered_map<model::Character*, std::shared_ptr<CharacterView>>
      _characters_views;
};
}  // namespace ui
