#pragma once
#include <imgui.h>
#include <map>
#include <string>
namespace ui {
class UIOptions {
public:
  const float RIGHT_SIDEBAR_WIDTH = 450.0f;
  const float LEFT_SIDEBAR_WIDTH = 200.0f;
  const float STAGE_ASPECT = 1.5f;

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

  template <typename... T>
  void initialize_font(UIOptions::Font font, T&&... args) {
    ImGuiIO& io = ImGui::GetIO();
    assert(!_fonts.count(font) && "Font initialized already");
    _fonts[font] =
        io.Fonts->AddFontFromFileTTF(std::forward<decltype(args)>(args)...);
  }
  ImFont* get_font(Font);

  // Args, and argv
  const char* get_path_name();

private:
  bool _running;
  std::string _current_cateogry = "Motion";  // WARN: for debugging for now
  std::map<Font, ImFont*> _fonts;
  int _args;
  char** _argv;
};
}  // namespace ui
