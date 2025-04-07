#pragma once

#include <memory>
#include <unordered_map>
#include "editor/script_view.h"
#include "events/event_dispatcher.h"
#include "ui_options.h"
#include "utils/ID_manager.h"
namespace ui {
class BlockCanvas {
public:
  BlockCanvas(UIOptions&);
  void draw();

private:
  void handle_canvas_drop();

private:
  UIOptions& _options;
  // A Map from a character scripts to the character scripts
  std::unordered_map<model::IDManager::IDType,
                     std::vector<std::shared_ptr<ScriptView>>>
      _script_views;
  std::vector<common::EventDispatcher::TokenP> _tkns;
};
}  // namespace ui
