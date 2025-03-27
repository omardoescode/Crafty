#pragma once

#include <memory>
#include "character.h"
namespace ui {
class CharacterView {
public:
  CharacterView(std::shared_ptr<model::Character>);

private:
  std::shared_ptr<model::Character> _character;
};
}  // namespace ui
