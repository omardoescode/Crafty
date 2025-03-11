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

private:
  bool _running;
  std::string _current_cateogry = "Motion";  // WARN: for debugging for now
};
}  // namespace ui
