#pragma once

#include "ui_options.h"
namespace ui {
class CharacterManager {
private:
  const int LEFT_SECTION_WIDTH = 100;

public:
  CharacterManager(UIOptions&);
  void draw();

private:
  void draw_button();

private:
  UIOptions& _options;
};
}  // namespace ui
