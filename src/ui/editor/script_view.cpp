#include "editor/script_view.h"
#include <unistd.h>
#include <memory>
#include "events/event_dispatcher.h"
#include "events/events.h"
#include "imgui.h"
#include "project_manager.h"
#include "ui_logger.h"
#include "ui_options.h"

static auto& mgr = model::ProjectManager::instance();
static auto& dispatcher = common::EventDispatcher::instance();

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
  dispatcher.subscribe<model::events::onBlockInstanceAddToScript>(
      [this](std::shared_ptr<model::events::onBlockInstanceAddToScript> evt) {
        if (evt->script->id() != _script->id()) return;
        instances_views[evt->instance->id()] =
            std::make_shared<BlockView>(_options, evt->instance, false);
        ui_logger("HELLO");
      });
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
    draw_drop_invisible_btn(i + 1);
    ImGui::PopID();
  }

  ImGui::PopStyleVar();
  ImGui::EndChild();
}

void ScriptView::draw_drop_invisible_btn(int position) {
  auto current_cursor = ImGui::GetCursorScreenPos();
  ImGui::SetCursorScreenPos(
      ImVec2(current_cursor.x, current_cursor.y - DRAG_BTN_HEIGHT / 2.f));

  ImGui::InvisibleButton(
      "", ImVec2(ImGui::GetContentRegionAvail().x, DRAG_BTN_HEIGHT));
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("BlockInstance")) {
      std::shared_ptr<model::BlockInstance> instance =
          *static_cast<std::shared_ptr<model::BlockInstance>*>(payload->Data);

      mgr.add_block_to_existing_script(_script->id(), instance->def(),
                                       position);
    }
    ImGui::EndDragDropTarget();
  }

  ImGui::SetCursorScreenPos(current_cursor);
}
}  // namespace ui
