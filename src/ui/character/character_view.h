#pragma once

#include <memory>
#include "character.h"
#include "ui_options.h"
namespace ui {
class CharacterView {
public:
  CharacterView(UIOptions& options, std::shared_ptr<model::Character>);

private:
  std::shared_ptr<model::Character> _character;
  UIOptions& _options;
};
}  // namespace ui
