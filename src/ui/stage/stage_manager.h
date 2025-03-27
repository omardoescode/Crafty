#pragma once

#include <memory>
#include <vector>
#include "character.h"
#include "character/character_view.h"
namespace ui {

class StageManager {
public:
  // remove copying
  StageManager(StageManager&) = delete;
  StageManager& operator=(StageManager&) = delete;

  static StageManager& instance();

  void add_character(std::shared_ptr<model::Character> character);

private:
  StageManager() = default;

private:
  std::vector<std::shared_ptr<CharacterView>> _character_views;
};

}  // namespace ui
