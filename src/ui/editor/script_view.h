#pragma once

#include <unordered_map>
#include "editor/block_view.h"
#include "script.h"
#include "ui_options.h"
#include "utils/ID_manager.h"
namespace ui {
class ScriptView {
public:
  ScriptView(UIOptions&, std::shared_ptr<model::Script>);
  void draw();

private:
  UIOptions& _options;
  std::shared_ptr<model::Script> _script;
  std::unordered_map<model::IDManager::IDType, std::shared_ptr<BlockView>>
      instances_views;
  ImVec2 _pos;
};
}  // namespace ui
