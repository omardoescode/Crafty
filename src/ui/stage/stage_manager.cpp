#include "stage_manager.h"
#include <memory>
#include "ui_options.h"
namespace ui {

StageManager& StageManager::instance() {
  static StageManager instance;
  return instance;
}

void StageManager::add_character(UIOptions& options,
                                 std::shared_ptr<model::Character> character) {
  _character_views.push_back(
      std::make_shared<CharacterView>(options, character));
}
}  // namespace ui
