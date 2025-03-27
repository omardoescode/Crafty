#pragma once

#include <filesystem>
#include <memory>
#include <mutex>
#include <vector>
#include "character/character_miniview.h"
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
  std::vector<std::shared_ptr<CharacterMiniView>> _miniviews;
  std::mutex _miniviews_mtx;
};
}  // namespace ui
