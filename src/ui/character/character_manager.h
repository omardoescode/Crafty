#pragma once

#include <filesystem>
#include <memory>
#include <mutex>
#include <unordered_map>
#include "character.h"
#include "character/character_miniview.h"
#include "events/event_dispatcher.h"
#include "ui_options.h"
namespace ui {
class CharacterManager {
private:
  const int LEFT_SECTION_WIDTH = 100;

public:
  CharacterManager(UIOptions&);
  ~CharacterManager();
  void draw();

private:
  void handle_add_click();
  void upload_char(std::filesystem::path);

private:
  UIOptions& _options;
  std::unordered_map<model::Character*, std::shared_ptr<CharacterMiniView>>
      _miniviews;
  // std::unordered_map<std::remove_const_t<std::remove_reference_t<
  //                        decltype(std::declval<model::Character>().id())>>,
  //                    std::shared_ptr<CharacterMiniView>>
  //     _miniviews;
  std::mutex _miniviews_mtx;
  std::vector<common::EventDispatcher::TokenP> _tkns;
};
}  // namespace ui
