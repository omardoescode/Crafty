#include "character_view.h"
#include "imgui.h"
#include "project_manager.h"
#include "utils/images.h"
auto& mgr = model::ProjectManager::instance();
namespace ui {
CharacterView::CharacterView(UIOptions& options,
                             std::shared_ptr<model::Character> character)
    : _character(character), _options(options), out_texture(0) {}
void CharacterView::draw() {
  load_texture_once();

  auto [x, y] = _character->pos();
  auto wid = _character->width();
  auto [res_x, res_y] = _options.stage_cords();
  x *= res_x / mgr.world_resolution.first;
  y *= res_y / mgr.world_resolution.second;
  wid *= res_x / mgr.world_resolution.first;

  ImGui::SetCursorPos(ImVec2(x, y));
  ImGui::Image((ImTextureID)(intptr_t)out_texture,
               ImVec2(wid, wid * out_height / out_width));
}

void CharacterView::load_texture_once() {
  if (out_texture) return;

  auto asset = mgr.character_current_sprite(_character);
  auto path = asset->get_path().c_str();
  bool res = LoadTextureFromFile(path, &out_texture, &out_width, &out_height);

  assert(res && "Failed to load texture");
}
}  // namespace ui
