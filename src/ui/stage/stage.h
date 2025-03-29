#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include "character/character_view.h"
#include "ui_options.h"
#include "utils/ID_manager.h"

namespace ui {
class Stage {
public:
  Stage(UIOptions& options, SDL_Renderer* renderer);
  void draw();

private:
  UIOptions& _options;

  // Model
  std::unordered_map<model::IDManager::IDType, std::shared_ptr<CharacterView>>
      _characters_views;
};
}  // namespace ui
