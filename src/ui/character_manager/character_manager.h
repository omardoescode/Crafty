#pragma once

#include <filesystem>
#include "ui_options.h"
namespace ui {
class CharacterManager {
private:
  const int LEFT_SECTION_WIDTH = 100;

public:
  CharacterManager(UIOptions&);
  void draw();

private:
  void upload_char(std::filesystem::path);

private:
  UIOptions& _options;
};
}  // namespace ui
