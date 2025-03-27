#include "character_miniview.h"
#include <cassert>
#include "SDL3/SDL_opengl.h"
#include "imgui.h"
#include "project_manager.h"
#include "ui_options.h"
#include "utils/images.h"

namespace ui {
CharacterMiniView::CharacterMiniView(
    UIOptions& options, std::shared_ptr<model::Character> character)
    : _options(options), _character(character), out_texture(0) {
  assert(!!character && "Nullptr to character");
}
CharacterMiniView::~CharacterMiniView() {
  if (out_texture) glDeleteTextures(1, &out_texture);
}

void CharacterMiniView::draw() {
  load_texture_once();
  ImGui::Image((ImTextureID)(intptr_t)out_texture, ImVec2(sz_x, sz_y));
}

void CharacterMiniView::load_texture_once() {
  if (out_texture) return;

  auto& mgr = model::ProjectManager::instance();
  auto asset = mgr.character_current_sprite(_character);
  bool res = LoadTextureFromFile(asset->get_path().c_str(), &out_texture,
                                 &out_width, &out_height);

  assert(res && "Failed to load texture");
}
}  // namespace ui
