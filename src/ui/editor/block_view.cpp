#include "block_view.h"
#include <algorithm>
#include <cstring>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include "block/block_instance.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "ui_options.h"

namespace ui {
// TODO: Refactor this mess
BlockView::BlockView(UIOptions& options,
                     std::shared_ptr<model::BlockInstance> instance,
                     bool draggable)
    : _options(options), _block_instance(instance), _draggable(draggable) {
  auto def = instance->def();
  std::stringstream name_stream(def->name());
  std::string word;
  std::regex slot_reg("^\\%(\\d+)$");  // TODO: Place this regex in model layer
  std::smatch match;
  std::string cur_word;
  while (name_stream >> word) {
    if (std::regex_search(word, match, slot_reg)) {
      if (!cur_word.empty()) {
        BlockPart part;
        part.type = BlockPart::BlockText;
        part.value = cur_word;
        _parts.emplace_back(std::move(part));
        cur_word.clear();
      }
      BlockPart part;
      part.type = BlockPart::InputSlot;
      part.value = _inner_views.size();
      part.direct_value =
          def->inputs().at(std::stoi(match.str(1))).default_value;
      // TODO: Fix this  to use the type of the
      // corresponding input slot
      _parts.push_back(part);  // For Now, there will be no current
                               // input slot instance attached
      _inner_views.push_back(nullptr);
    } else {
      if (!cur_word.empty()) cur_word += " ";
      cur_word += word;
    }
  }
  if (!cur_word.empty()) {
    BlockPart part;
    part.type = BlockPart::BlockText;
    part.value = cur_word;
    _parts.emplace_back(std::move(part));
    cur_word.clear();
  }
}
void BlockView::draw() {
  auto before_draw = ImGui::GetCursorScreenPos();

  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(40, 40, 40, 255));
  ImGui::BeginChild(_block_instance->def()->id()->to_string().c_str(),
                    ImVec2(0, 0),
                    ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);

  // Handle Drag and Drop
  handle_drag();

  // Insert Top Padding
  ImGui::Dummy(ImVec2(0, PADDING.second));

  // Insert Left Padding
  ImGui::Dummy(ImVec2(PADDING.first, 0));
  ImGui::SameLine();

  // Draw the actual thing
  draw_parts();
  ImGui::SameLine();

  // Insert Right Padding
  ImGui::Dummy(ImVec2(PADDING.first, 0));

  // Insert bottom Padding
  ImGui::Dummy(ImVec2(0, PADDING.second));

  ImGui::PopStyleColor();
  ImGui::EndChild();
}

void BlockView::handle_drag() {
  if (!_draggable) return;
  ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  // TODO: Move the cursor to be in the middle of the block
  if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
    ImGui::SetDragDropPayload("BlockInstance", &_block_instance,
                              sizeof(_block_instance));
    draw();  // draw the same thing over, bro this is genius xD
    ImGui::EndDragDropSource();
  }
  ImGui::PopStyleVar(2);
}

void BlockView::draw_parts() {
  for (int i = 0; i < _parts.size(); i++) {
    auto& [type, value, direct_value] = _parts[i];
    switch (type) {
      case BlockPart::BlockText: {
        std::string text = std::get<std::string>(value);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", text.c_str());
      } break;
      case BlockPart::InputSlot:
        ImGui::PushID(i);
        ImGui::PushStyleVar(
            ImGuiStyleVar_FramePadding,
            ImVec2(DIRECT_VALUE_PADDING.first, DIRECT_VALUE_PADDING.second));
        ImGui::SetNextItemWidth(
            std::max(2 * DIRECT_VALUE_PADDING.first +
                         ImGui::CalcTextSize(direct_value.c_str()).x,
                     40.f));
        ImGui::InputText("", &direct_value, ImGuiInputTextFlags_CharsDecimal);
        ImGui::PopStyleVar();
        ImGui::PopID();
        break;
    }
    ImGui::SameLine();
  }
}
}  // namespace ui
