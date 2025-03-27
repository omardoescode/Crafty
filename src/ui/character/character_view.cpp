#include "character_view.h"
namespace ui {
CharacterView::CharacterView(UIOptions& options,
                             std::shared_ptr<model::Character> character)
    : _character(character), _options(options) {}
}  // namespace ui
