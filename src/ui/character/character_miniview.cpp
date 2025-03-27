#include "character_miniview.h"
#include <cassert>
#include <iostream>
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

  // Style parameters
  const float rounding = 3.0f;
  const ImU32 color = IM_COL32(128, 128, 128, 255);
  const ImU32 hovered_color = IM_COL32(192, 192, 192, 255);

  const ImVec2 padding = ImVec2(10, 10);

  ImGui::PushID(_character->id().c_str());  // Ensure unique ID

  // Calculate total size
  const ImVec2 image_size(sz_x, sz_y);
  const ImVec2 total_size =
      ImVec2(image_size.x + padding.x * 2,
             image_size.y + ImGui::GetTextLineHeight() + padding.y * 3);

  // Create an invisible button for interaction
  ImGui::InvisibleButton("character_card", total_size);
  const bool is_hovered = ImGui::IsItemHovered();
  const bool is_clicked = ImGui::IsItemClicked();

  // Draw rounded rectangle background
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  const ImVec2 min = ImGui::GetItemRectMin();
  const ImVec2 max = ImGui::GetItemRectMax();

  draw_list->AddRectFilled(
      min, max, ImGui::GetColorU32(is_hovered ? hovered_color : color),
      rounding);

  // Draw image with padding
  ImGui::SetCursorScreenPos(ImVec2(min.x + padding.x, min.y + padding.y));
  ImGui::Image((ImTextureID)(intptr_t)out_texture, image_size, ImVec2(0, 0),
               ImVec2(1, 1));

  // Draw character name below image
  ImGui::SetCursorScreenPos(
      ImVec2(min.x + padding.x, min.y + image_size.y + padding.y * 2));
  ImGui::TextWrapped("%s", _character->name().c_str());

  // Handle click
  if (is_clicked) {
    std::cout << "clicked" << std::endl;
  }

  ImGui::PopID();
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
