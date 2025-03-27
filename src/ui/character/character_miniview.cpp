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

  // Constants for card dimensions, image size, and padding
  const float PADDING = 10.0f;

  // Style parameters
  const float rounding = 3.0f;
  const ImU32 color = IM_COL32(128, 128, 128, 255);
  const ImU32 active_color = IM_COL32(255, 200, 100, 255);
  const ImU32 hovered_color = IM_COL32(192, 192, 192, 255);

  ImGui::PushID(_character->id().c_str());  // Ensure unique ID

  // Use fixed card size for the invisible button
  ImGui::InvisibleButton("character_card", ImVec2(CARD_WIDTH, CARD_HEIGHT));
  const bool is_hovered = ImGui::IsItemHovered();
  const bool is_clicked = ImGui::IsItemClicked();

  // Get the position and size of the invisible button
  const ImVec2 min = ImGui::GetItemRectMin();
  const ImVec2 max = ImGui::GetItemRectMax();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  // Draw rounded rectangle background
  ImU32 col;
  if (is_hovered)
    col = hovered_color;
  else if (_options.current_character() == _character)
    col = active_color;
  else
    col = color;

  draw_list->AddRectFilled(min, max, ImGui::GetColorU32(col), rounding);

  // Position the image centered horizontally at the top with padding
  const ImVec2 image_top_left(min.x + (CARD_WIDTH - IMAGE_X) / 2.0f,
                              min.y + PADDING);
  ImGui::SetCursorScreenPos(image_top_left);
  ImGui::Image((ImTextureID)(intptr_t)out_texture, ImVec2(IMAGE_X, IMAGE_Y));

  // Calculate position for the character name
  const std::string name = _character->name();
  const float available_text_width = CARD_WIDTH - 2 * PADDING;
  ImVec2 text_size =
      ImGui::CalcTextSize(name.c_str(), nullptr, false, available_text_width);

  // Center the text horizontally and position below the image
  const ImVec2 text_pos(min.x + (CARD_WIDTH - text_size.x) / 2.0f,
                        image_top_left.y + IMAGE_Y + PADDING);

  ImGui::SetCursorScreenPos(text_pos);
  ImGui::TextWrapped("%s", name.c_str());

  // Handle click
  if (is_clicked) {
    _options.set_current_character(_character);
  }

  ImGui::SetCursorScreenPos(
      ImVec2(image_top_left.x + CARD_WIDTH, image_top_left.y + CARD_HEIGHT));
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
