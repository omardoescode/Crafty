#pragma once

#include "ui_options.h"
namespace ui {
class CharacterManager {
public:
  CharacterManager(UIOptions&);
  void draw();

private:
  UIOptions& _options;
};
}  // namespace ui
