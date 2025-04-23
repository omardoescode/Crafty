#include "character_view.h"
#include <algorithm>
#include "asset.h"
#include "imgui.h"
#include "project_manager.h"
#include "ui_logger.h"
#include "utils/images.h"
auto& mgr = model::ProjectManager::instance();
namespace ui {
CharacterView::CharacterView(UIOptions& options,
                             std::shared_ptr<model::Character> character)
    : _character(character), _options(options), out_texture(0) {}
void CharacterView::draw() {
  load_texture_once();

  auto [world_x, world_y] = _character->pos();
  float wid = _character->width(), hei;
  auto [res_x, res_y] = _options.stage_cords();
  float screen_x = world_x * res_x / mgr.world_resolution.first;
  float screen_y = world_y * res_y / mgr.world_resolution.second;

  wid *= res_x / mgr.world_resolution.first;

  hei = wid * out_height / out_width;

  ImGui::SetCursorPos(ImVec2(screen_x, screen_y));
  ImGui::InvisibleButton("##drag_handle", ImVec2(wid, hei));
  ImGui::GetWindowDrawList()->AddCircle(ImGui::GetMousePos(), 5.0f,
                                        IM_COL32(255, 0, 0, 255));
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
    ImVec2 mouse_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);

    // Calculate new position in world (backend) coordinates
    const float scale_x = res_x / mgr.world_resolution.first;
    const float scale_y = res_y / mgr.world_resolution.second;
    const float uniform_scale = std::min(scale_x, scale_y);

    float new_x = world_x + mouse_delta.x / uniform_scale;
    float new_y = world_y + mouse_delta.y / uniform_scale;

    // Apply boundary constraints
    const float max_x = mgr.world_resolution.first - _character->width();
    const float max_y = mgr.world_resolution.second -
                        (_character->width() * out_height / out_width);
    new_x = std::clamp(new_x, 0.0f, max_x);
    new_y = std::clamp(new_y, 0.0f, max_y);

    // Update character position
    _character->set_pos({new_x, new_y});

    // Reset drag delta (important for smooth dragging)
    ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
  }
  if (ImGui::IsItemHovered()) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
  }

  ImVec2 top_left = ImGui::GetItemRectMin();
  ImVec2 center = ImVec2(top_left.x + wid * 0.5f, top_left.y + hei * 0.5f);

  ImageRotated((ImTextureID)(intptr_t)out_texture, center, ImVec2(wid, hei),
               _character->rotation());
}

void CharacterView::load_texture_once() {
  if (out_texture) return;

  auto asset = _character->current_sprite();
  auto path = asset->get_path().c_str();
  bool res = LoadTextureFromFile(path, &out_texture, &out_width, &out_height);

  if (!res) {
    throw ui_logger().error("Failed to load texture from path: {}",
                            path);  // TODO: Handle better
  }
}
}  // namespace ui
