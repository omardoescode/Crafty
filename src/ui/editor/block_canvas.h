#pragma once

#include "ui_options.h"
namespace ui {
class BlockCanvas {
public:
  BlockCanvas(UIOptions&);
  void draw();

private:
  void handle_canvas_drop();

private:
  UIOptions& _options;
};
}  // namespace ui
