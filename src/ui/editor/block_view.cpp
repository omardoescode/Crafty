#include "block_view.h"
#include <memory>
#include "block/block_instance.h"
#include "imgui.h"
#include "ui_options.h"

namespace ui {
BlockView::BlockView(UIOptions& options,
                     std::shared_ptr<model::BlockInstance> instance)
    : _options(options), _block_instance(instance) {}

void BlockView::draw() {
  static int n = 3;
  const ImU32 color = IM_COL32(77, 166, 255, 255);
  const ImU32 hovered_color = IM_COL32(107, 186, 255, 255);  // Lighter blue
  const char* text = _block_instance->def()->id().c_str();
  ImVec2 text_size = ImGui::CalcTextSize(text);
  ImVec2 total_size =
      ImVec2(text_size.x + 2 * PADDING.x, text_size.y + 2 * PADDING.y);

  // Reserve space (this informs the table cell's size)
  ImGui::InvisibleButton(text, total_size);  // The ID `text` has to be unique
  bool is_hovered = ImGui::IsItemHovered();
  auto btn_color = is_hovered ? hovered_color : color;
  if (is_hovered) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

  ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  if (ImGui::BeginDragDropSource()) {
    ImGui::SetDragDropPayload("BlockInstance", &_block_instance,
                              sizeof(_block_instance));
    draw();  // draw the same thing over, bro this is genius xD
    ImGui::EndDragDropSource();
  }
  ImGui::PopStyleVar();
  ImGui::PopStyleVar();

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
