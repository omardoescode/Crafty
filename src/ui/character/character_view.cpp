#include "character_view.h"
namespace ui {
CharacterView::CharacterView(std::shared_ptr<model::Character> character)
    : _character(character) {}
}  // namespace ui
