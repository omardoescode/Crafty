#include "BlockView.h"
#include <memory>
#include "block/BlockDefinition.h"
#include "block/BlockInstance.h"
#include "imgui.h"
#include "ui_options.h"

namespace ui {
BlockView::BlockView(UIOptions& options,
                     std::shared_ptr<model::BlockInstance> instance)
    : _options(options), _block_instance(instance) {}

void BlockView::draw() {
  const ImU32 color = IM_COL32(77, 166, 255, 255);
  const ImU32 hovered_color = IM_COL32(107, 186, 255, 255);  // Lighter blue
  const char* text = _block_instance->definition_id.c_str();
  ImVec2 text_size = ImGui::CalcTextSize(text);
  ImVec2 total_size =
      ImVec2(text_size.x + 2 * PADDING.x, text_size.y + 2 * PADDING.y);

  // Reserve space (this informs the table cell's size)
  ImGui::InvisibleButton(text, total_size);  // The ID `text` has to be unique
  bool is_hovered = ImGui::IsItemHovered();
  auto btn_color = is_hovered ? hovered_color : color;
  if (is_hovered) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

  // Draw over the reserved space
  ImVec2 rect_min = ImGui::GetItemRectMin();
  ImVec2 rect_max =
      ImVec2(rect_min.x + total_size.x, rect_min.y + total_size.y);
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  // Background rectangle
  draw_list->AddRectFilled(rect_min, rect_max, btn_color, ROUNDING);

  // Text
  ImVec2 text_pos = ImVec2(rect_min.x + PADDING.x, rect_min.y + PADDING.y);
  draw_list->AddText(text_pos, IM_COL32_WHITE, text);
}

}  // namespace ui
