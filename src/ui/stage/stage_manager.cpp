#include "stage_manager.h"
#include <memory>
namespace ui {

StageManager& StageManager::instance() {
  static StageManager instance;
  return instance;
}

void StageManager::add_character(std::shared_ptr<model::Character> character) {
  _character_views.push_back(std::make_shared<CharacterView>(character));
}
}  // namespace ui
