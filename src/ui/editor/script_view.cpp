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
  for (const auto& block_id : _script->blocks()) {
    ImGui::PushID(&block_id);
    instances_views[block_id]->draw();
    ImGui::PopID();
  }

  // 2. Draw Invisible buttons to drop new blocks on
  // TODO:

  ImGui::PopStyleVar();
  ImGui::EndChild();
}

void ScriptView::draw_drop_invisible_btn() {
  auto current_cursor = ImGui::GetCursorPos();
  ImGui::SetCursorPos(
      ImVec2(current_cursor.x, current_cursor.y -= DRAG_BTN_HEIGHT));
  ImGui::Button("##DUMMY", ImVec2(DRAG_BTN_WIDTH, DRAG_BTN_HEIGHT));
}
}  // namespace ui
