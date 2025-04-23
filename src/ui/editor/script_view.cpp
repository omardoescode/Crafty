#include "editor/script_view.h"
#include <unistd.h>
#include <memory>
#include "events/event_dispatcher.h"
#include "imgui.h"
#include "model_events.h"
#include "project_manager.h"
#include "ui_options.h"

static auto& mgr = model::ProjectManager::instance();
static auto& dispatcher = common::EventDispatcher::instance();

namespace ui {
ScriptView::ScriptView(UIOptions& options,
                       std::shared_ptr<model::Script> script)
    : _options(options), _script(script) {
  // 1. Create views for the current instances
  for (const auto& instance : _script->blocks()) {
    _instances_views[instance.get()] =
        std::make_shared<BlockView>(_options, instance, false);
  }

  // 2. Add a subscriber to add future blocks
  _instance_add_tkn =
      dispatcher.subscribe<model::events::onBlockInstanceAddToScript>(
          [this](
              std::shared_ptr<model::events::onBlockInstanceAddToScript> evt) {
            if (evt->script != _script) return;
            _instances_views[evt->instance.get()] =
                std::make_shared<BlockView>(_options, evt->instance, false);
          });
}
void ScriptView::draw() {
  ImGui::SetCursorScreenPos(
      ImVec2(_script->pos().first, _script->pos().second));
  ImGui::BeginChild("", ImVec2(0, 0),
                    ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 3));
  assert(_script->has_block_instances() &&
         "Invalid script. A script must have at least one block instance");

  // 1. Draw the scripts
  const auto& blocks = _script->blocks();
  for (int i = 0; i < blocks.size(); i++) {
    const auto& block = blocks[i];

    ImGui::PushID(block.get());
    _instances_views[block.get()]->draw();
    ImGui::PopID();

    ImGui::PushID(block.get());
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

      mgr.add_block_to_existing_script(_script, instance->def(), position);
    }
    ImGui::EndDragDropTarget();
  }

  ImGui::SetCursorScreenPos(current_cursor);
}
}  // namespace ui
