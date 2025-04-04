#include "editor/script_view.h"
#include <unistd.h>
#include <memory>
#include "imgui.h"
#include "project_manager.h"
#include "ui_logger.h"
#include "ui_options.h"

static auto& mgr = model::ProjectManager::instance();

namespace ui {
ScriptView::ScriptView(UIOptions& options,
                       std::shared_ptr<model::Script> script)
    : _options(options), _script(script) {
  // 1. Create views for the current instances
  for (const auto& block_id : _script->blocks()) {
    auto instance = mgr.project()->instances_store().get_entity(block_id);
    instances_views[block_id] =
        std::make_shared<BlockView>(_options, instance, false);
  }

  // 2. TODO: Subscribe to instanceAddedToScript event
}
void ScriptView::draw() {
  ImGui::SetCursorScreenPos(
      ImVec2(_script->pos().first, _script->pos().second));
  ImGui::BeginChild(_script->id().c_str(), ImVec2(0, 0),
                    ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 3));
  assert(_script->has_block_instances() &&
         "Invalid script. A script must have at least one block instance");

  // 1. Draw the scripts
  const auto& block_ids = _script->blocks();
  for (int i = 0; i < block_ids.size(); i++) {
    const auto& block_id = block_ids[i];
    ImGui::PushID(i);
    instances_views[block_id]->draw();
    ImGui::PopID();

    ImGui::PushID(i);
    draw_drop_invisible_btn();
    ImGui::PopID();
  }

  ImGui::PopStyleVar();
  ImGui::EndChild();
}

void ScriptView::draw_drop_invisible_btn() {
  auto current_cursor = ImGui::GetCursorScreenPos();
  ImGui::SetCursorScreenPos(
      ImVec2(current_cursor.x, current_cursor.y - DRAG_BTN_HEIGHT / 2.f));

  ImGui::InvisibleButton(
      "", ImVec2(ImGui::GetContentRegionAvail().x, DRAG_BTN_HEIGHT));
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("BlockInstance")) {
      // TODO: Add script to view
    }
    ImGui::EndDragDropTarget();
  }

  ImGui::SetCursorScreenPos(current_cursor);
}
}  // namespace ui
