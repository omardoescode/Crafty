#pragma once
// Provide Settings for the UI
#include <string>
#include "imgui.h"
namespace ui {
class UIOptions {
public:
  /**
   * @brief Constructor
   *
   * Initialize the various variables
   * TODO: Load Future Layout from a file
   */
  UIOptions();

  // App State
  const bool& running() const;
  void close();

  // Block Editor state
  const std::string current_category() const;
  void set_current_category(const std::string&);

  // Helpful stuff for colors
  // Get the hover color by decreasing alpha by -.1f
  ImVec4 get_hover_color(ImVec4);

private:
  bool _running;
  std::string _current_cateogry;
};
}  // namespace ui
