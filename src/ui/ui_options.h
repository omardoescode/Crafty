#pragma once
#include <imgui.h>
#include <filesystem>
#include <map>
#include <string>
#include "character.h"
namespace ui {
class UIOptions {
public:
  const float RIGHT_SIDEBAR_WIDTH = 450.0f;
  const float LEFT_SIDEBAR_WIDTH = 200.0f;
  const float STAGE_ASPECT = 1.5f;

  // variable sized global variable
  float rounding = 3.0f;

  enum Font {
    DEFAULT_FONT,
    ICONS_FONT_SMALL,
    ICONS_FONT_MEDIUM,
    ICONS_FONT_BIG
  };

public:
  /**
   * @brief Constructor
   *
   * Initialize the various variables
   * TODO: Load Future Layout from a file
   */
  UIOptions(int args, char** argv);

  // App State
  const bool& running() const;
  void close();

  // Block Editor state
  const std::string current_category() const;
  void set_current_category(const std::string&);

  const std::shared_ptr<model::Character> current_character() const;
  void set_current_character(std::shared_ptr<model::Character>);

  template <typename... T>
  void initialize_font(UIOptions::Font font, T&&... args) {
    ImGuiIO& io = ImGui::GetIO();
    assert(!_fonts.count(font) && "Font initialized already");
    _fonts[font] =
        io.Fonts->AddFontFromFileTTF(std::forward<decltype(args)>(args)...);
  }
  ImFont* get_font(Font);

  // Get Directories
  /**
   * Get the folder where all assets are copied to
   *
   * NOTE: Must be called on the main thread or a non-detached thread
   */
  std::filesystem::path executable_path();
  std::filesystem::path asset_dest_folder();

  void set_stage_width(float);
  std::pair<float, float> stage_cords() const;

private:
  bool _running;
  std::string _current_cateogry = "Motion";  // WARN: for debugging for now
  std::map<Font, ImFont*> _fonts;
  int _args;
  char** _argv;
  std::filesystem::path _path_name;
  std::shared_ptr<model::Character> _current_character;
  float _stage_width;
};
}  // namespace ui
