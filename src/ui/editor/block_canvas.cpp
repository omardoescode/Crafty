#include "block_canvas.h"
#include <algorithm>
#include <memory>
#include "block/block_instance.h"
#include "events/event_dispatcher.h"
#include "events/events.h"
#include "imgui.h"
#include "project_manager.h"
#include "ui_options.h"

namespace ui {
BlockCanvas::BlockCanvas(UIOptions& options) : _options(options) {
  auto& dispatcher = common::EventDispatcher::instance();
  dispatcher.subscribe<model::events::onScriptCreated>(
      [this](std::shared_ptr<model::events::onScriptCreated> evt) {
        _script_views[evt->script->character()->id()].push_back(
            std::make_shared<ScriptView>(_options, evt->script));
      });
}

void BlockCanvas::draw() {
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  // Canvas setup (pan/zoom logic)
  ImVec2 canvas_p0;  // Screen-space canvas origin
  ImVec2 canvas_size = ImGui::GetContentRegionAvail();
  ImVec2 offset = ImVec2(0, 0);  // Pan offset
  float zoom = 1.0f;

  // Handle panning
  if (ImGui::IsMouseDragging(
          ImGuiMouseButton_Left)) {  // TODO: use left button to drag?
    offset.x += ImGui::GetIO().MouseDelta.x;
    offset.y += ImGui::GetIO().MouseDelta.y;
    // TODO: Figure out to how to use this offset
  }

  // Handle zoom (mouse wheel)
  // TODO: Render ZOOM?
  zoom += ImGui::GetIO().MouseWheel * 0.1f;
  zoom = std::clamp(zoom, 0.1f, 3.0f);

  // Draw canvas background
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  canvas_p0 = ImGui::GetCursorScreenPos();
  draw_list->AddRectFilled(
      canvas_p0,
      ImVec2(canvas_p0.x + canvas_size.x, canvas_p0.y + canvas_size.y),
      IM_COL32(30, 50, 50, 255));

  // Capture the beginning before drawing the dummy button
  auto cursor_before_btn = ImGui::GetCursorScreenPos();

  // Handle Drop logic
  ImGui::InvisibleButton("##CanvasDummy", canvas_size);
  handle_canvas_drop();

  // Capture the end of the inviside button
  auto cursor_after_btn = ImGui::GetCursorScreenPos();

  // Draw the script views
  // 1. Set the cursor back first
  ImGui::SetCursorScreenPos(cursor_before_btn);

  // 2. Draw the scripts if any
  if (_options.current_character()) {
    auto& current_character_scripts =
        _script_views[_options.current_character()->id()];
    for (auto& script : current_character_scripts) {
      script->draw();
    }
  }

  // 3. Return the cursor back
  ImGui::SetCursorScreenPos(cursor_after_btn);
  ImGui::PopStyleVar(1);
}
void BlockCanvas::handle_canvas_drop() {
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(
            "BlockInstance", ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {
      // Success! Handle the payload
      // Access payload data:
      std::shared_ptr<model::BlockInstance> instance =
          *static_cast<std::shared_ptr<model::BlockInstance>*>(payload->Data);

      // Get the Mouse positon
      auto pos = ImGui::GetIO().MousePos;

      // Create a script
      auto& mgr = model::ProjectManager::instance();
      mgr.add_script(_options.current_character(), instance->def(), pos.x,
                     pos.y);
    }
    ImGui::EndDragDropTarget();
  }
}
}  // namespace ui
