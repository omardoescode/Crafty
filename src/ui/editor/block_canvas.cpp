#include "block_canvas.h"
#include <memory>
#include "block/block_instance.h"
#include "events/event_dispatcher.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "model_events.h"
#include "project_manager.h"
#include "ui_logger.h"
#include "ui_options.h"
#include "utils/material_symbols.h"

namespace ui {
BlockCanvas::BlockCanvas(UIOptions& options)
    : _options(options), _offset({0, 0}) {
  auto& dispatcher = common::EventDispatcher::instance();
  _tkns.emplace_back(dispatcher.subscribe<model::events::onScriptCreated>(
      [this](std::shared_ptr<model::events::onScriptCreated> evt) {
        _script_views[evt->character.get()].push_back(
            std::make_shared<ScriptView>(_options, evt->script));
      }));
}

void BlockCanvas::draw() {
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  // Canvas setup (pan/zoom logic)
  ImVec2 canvas_p0;  // Screen-space canvas origin
  ImVec2 canvas_size = ImGui::GetContentRegionAvail();
  static float zoom = 1.0f;

  // Handle panning
  if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) &&
      !ImGui::IsDragDropActive()) {
    _offset.x += ImGui::GetIO().MouseDelta.x;
    _offset.y += ImGui::GetIO().MouseDelta.y;
    ui_logger().info("Offset: ({}, {})", _offset.x, _offset.y);
  }

  // Draw canvas background
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  canvas_p0 = ImGui::GetCursorScreenPos();
  ImVec2 offsetted_canvas_p0(canvas_p0.x + _offset.x, canvas_p0.y + _offset.y);
  draw_list->AddRectFilled(
      canvas_p0,
      ImVec2(canvas_p0.x + canvas_size.x, canvas_p0.y + canvas_size.y),
      IM_COL32(30, 50, 50, 255));

  // Capture the beginning before drawing the dummy button
  auto cursor_before_btn = ImGui::GetCursorScreenPos();

  // Handle Drop logic
  ImGui::InvisibleButton("##CanvasDummy", canvas_size);
  handle_canvas_drop(canvas_p0);

  // Capture the end of the inviside button
  auto cursor_after_btn = ImGui::GetCursorScreenPos();

  // Draw the script views
  // 1. Set the cursor back first
  ImGui::SetCursorScreenPos(cursor_before_btn);

  // 2. Draw the scripts if any
  if (_options.current_character()) {
    auto& current_character_scripts =
        _script_views[_options.current_character().get()];
    for (auto& chr :
         model::ProjectManager::instance().project()->characters()) {
      auto& scripts = _script_views[chr.get()];
      for (auto script : scripts) {
        ImGui::PushID(script.get());
        script->draw(offsetted_canvas_p0);
        ImGui::PopID();
      }
    }
  }

  // 3. Draw The Buttons on left bottom
  ImGui::SetCursorScreenPos(
      ImVec2(cursor_after_btn.x - 100, cursor_after_btn.y - 100));
  ImGui::PushFont(_options.get_font(UIOptions::Font::ICONS_FONT_MEDIUM));
  ImGui::Button(ICON_MD_ADD);
  ImGui::PopFont();

  // 4. Return the cursor back
  ImGui::SetCursorScreenPos(cursor_after_btn);
  ImGui::PopStyleVar(1);
}
void BlockCanvas::handle_canvas_drop(ImVec2 canvas_p0) {
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(
            "BlockInstance", ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {
      // Success! Handle the payload
      // Access payload data:
      std::shared_ptr<model::BlockInstance> instance =
          *static_cast<std::shared_ptr<model::BlockInstance>*>(payload->Data);

      // Get the Mouse positon
      auto pos = ImGui::GetIO().MousePos;
      pos.x -= _offset.x + canvas_p0.x;
      pos.y -= _offset.y + canvas_p0.y;

      // Create a script
      auto& mgr = model::ProjectManager::instance();
      mgr.add_script(_options.current_character(), instance->def(), pos.x,
                     pos.y);
    }
    ImGui::EndDragDropTarget();
  }
}
}  // namespace ui
