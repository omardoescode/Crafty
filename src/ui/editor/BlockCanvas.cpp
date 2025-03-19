#include "BlockCanvas.h"
#include <algorithm>
#include <iostream>
#include "imgui.h"
#include "ui_options.h"

namespace ui {
BlockCanvas::BlockCanvas(UIOptions& options) : _options(options) {}

void BlockCanvas::draw() {
  if (ImGui::BeginChild("Canvas")) {
    // Canvas setup (pan/zoom logic)
    static ImVec2 canvas_p0;  // Screen-space canvas origin
    static ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    static ImVec2 offset = ImVec2(0, 0);  // Pan offset
    static float zoom = 1.0f;

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

    // Handle Drop logic
    ImGui::InvisibleButton("##CanvasDummy", canvas_size);
    if (ImGui::BeginDragDropTarget()) {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(
              "BlockInstance", ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {
        // Success! Handle the payload
        std::cout << "DragDrop Success!" << std::endl;
        // Access payload data:
        // auto instance =
        // *static_cast<std::shared_ptr<model::BlockInstance>*>(payload->Data);
      }
      ImGui::EndDragDropTarget();
    }
    // End Canvas
    ImGui::EndChild();
  }
}
}  // namespace ui
