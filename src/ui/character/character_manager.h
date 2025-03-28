#pragma once

#include <filesystem>
#include <memory>
#include <mutex>
#include <type_traits>
#include <unordered_map>
#include "character.h"
#include "character/character_miniview.h"
#include "ui_options.h"
#include "utils/ID_manager.h"
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
  std::unordered_map<model::IDManager::IDType,
                     std::shared_ptr<CharacterMiniView>>
      _miniviews;
  // std::unordered_map<std::remove_const_t<std::remove_reference_t<
  //                        decltype(std::declval<model::Character>().id())>>,
  //                    std::shared_ptr<CharacterMiniView>>
  //     _miniviews;
  std::mutex _miniviews_mtx;
};
}  // namespace ui
