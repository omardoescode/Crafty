#pragma once

#include <memory>
#include <unordered_map>
#include "character.h"
#include "editor/script_view.h"
#include "events/event_dispatcher.h"
#include "ui_options.h"

namespace ui {
class BlockCanvas {
public:
  BlockCanvas(UIOptions&);
  void draw();

private:
  void handle_canvas_drop(ImVec2 canvas_p0);

private:
  UIOptions& _options;
  // A Map from a character scripts to the character scripts
  std::unordered_map<model::Character*,
                     std::vector<std::shared_ptr<ScriptView>>>
      _script_views;
  std::vector<common::EventDispatcher::TokenP> _tkns;
  ImVec2 _offset;  // Pan offset
};
}  // namespace ui
