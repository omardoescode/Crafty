#include "editor/script_view.h"
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
    instances_views[block_id] = std::make_shared<BlockView>(_options, instance);
  }

  // 2. Create the positon
  auto first_instance =
      mgr.project()->instances_store().get_entity(_script->blocks()[0]);
  _pos = {first_instance->x(), first_instance->y()};

  // 2. TODO: Subscribe to instanceAddedToScript event
}
void ScriptView::draw() {
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 3));
  assert(_script->has_block_instances() &&
         "Invalid script. A script must have at least one block instance");

  const auto& block_ids = _script->blocks();

  // 1. Set the cursor for the first block instance
  ImGui::SetCursorScreenPos(_pos);

  // 2. Draw the scripts
  for (const auto& block_id : _script->blocks()) {
    ImGui::PushID(&block_id);
    instances_views[block_id]->draw();
    ImGui::PopID();
  }

  ImGui::PopStyleVar();
}
}  // namespace ui
