#include "ui_options.h"
#include <filesystem>
#include <memory>
#include "events/event_dispatcher.h"
#include "events/events.h"
#include "utils/platform.h"
namespace ui {
UIOptions::UIOptions(int args, char** argv)
    : _running{true}, _args(args), _argv(argv) {
  _path_name = get_executable_path().parent_path();

  auto& dispatcher = common::EventDispatcher::instance();
  dispatcher.subscribe<model::events::beforeCharacterDeleted>(
      [this](std::shared_ptr<model::events::beforeCharacterDeleted> evt) {
        auto& chr = evt->character;
        if (chr == _current_character) _current_character = nullptr;
      });
}
const bool& UIOptions::running() const { return _running; }
void UIOptions::close() { _running = false; }

const std::string UIOptions::current_category() const {
  return _current_cateogry;
}
void UIOptions::set_current_category(const std::string& value) {
  _current_cateogry = value;
}

ImFont* UIOptions::get_font(UIOptions::Font font) {
  auto itr = _fonts.find(font);
  assert(itr != _fonts.end() && "Font is not initialized");
  return itr->second;
}

std::filesystem::path UIOptions::executable_path() { return _path_name; }

std::filesystem::path UIOptions::asset_dest_folder() {
  auto res =
      _path_name /
      "asset_dest";  // TODO: Create some UIConfig json to setup all of these
  std::filesystem::create_directory(res);  // create if doesn't exist
  return res;
}

const std::shared_ptr<model::Character> UIOptions::current_character() const {
  return _current_character;
}
void UIOptions::set_current_character(
    std::shared_ptr<model::Character> character) {
  _current_character = character;
}
void UIOptions::set_stage_width(float width) { _stage_width = width; }
std::pair<float, float> UIOptions::stage_cords() const {
  return {_stage_width, _stage_width / STAGE_ASPECT};
}
}  // namespace ui
