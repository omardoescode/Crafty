#pragma once

#include <unordered_map>
#include "editor/block_view.h"
#include "events/event_dispatcher.h"
#include "script.h"
#include "ui_options.h"
#include "utils/ID_manager.h"
namespace ui {
class ScriptView {
public:
  ScriptView(UIOptions&, std::shared_ptr<model::Script>);
  void draw();

private:
  constexpr static float DRAG_BTN_HEIGHT = 16.f;

  void draw_drop_invisible_btn(int position);

private:
  UIOptions& _options;
  std::shared_ptr<model::Script> _script;
  std::unordered_map<model::IDManager::IDType, std::shared_ptr<BlockView>>
      instances_views;
  common::EventDispatcher::TokenP _instance_add_tkn;
};
}  // namespace ui
