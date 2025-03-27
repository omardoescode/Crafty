#pragma once

#include "ui_options.h"
namespace ui {
class BlockCanvas {
public:
  BlockCanvas(UIOptions&);
  void draw();

private:
  UIOptions& _options;
};
}  // namespace ui
