#include "block_view.h"
#include <cstring>
#include <memory>
#include <regex>
#include <string>
#include <variant>
#include "block/block_instance.h"
#include "block/input_slot_instance.h"
#include "block/value.h"
#include "block/value_type.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "ui_options.h"

namespace ui {
BlockView::BlockView(UIOptions& options,
                     std::shared_ptr<model::BlockInstance> instance,
                     bool draggable)
    : _options(options),
      _block_instance(std::move(instance)),
      _draggable(draggable) {
  parse_block_name();
}

void BlockView::parse_block_name() {
  // Get the block name from the block instance
  const std::string& name = _block_instance->def()->name();

  // Create a vector to hold the parts
  std::vector<BlockPart> parts;

  // Parse the name and create parts
  handle_text(name, parts);

  // Store the parts
  _parts = std::move(parts);
}

void BlockView::handle_text(const std::string& text,
                            std::vector<BlockPart>& parts) {
  // Regular expression to match input slot placeholders (%0, %1, etc.)
  static const std::regex slot_reg(R"(\%\d+)");

  // Current position in the text
  size_t pos = 0;

  // Find all matches of the regex in the text
  std::sregex_iterator it(text.begin(), text.end(), slot_reg);
  std::sregex_iterator end;

  // Process each match
  while (it != end) {
    // Get the match
    std::smatch match = *it;

    // Add the text before the match as a string part
    if (match.position() > pos) {
      parts.emplace_back(text.substr(pos, match.position() - pos));
    }

    // Extract the slot index from the match
    std::string slot_str = match.str().substr(1);  // Remove the % character
    size_t slot_index = std::stoul(slot_str);

    // Add the slot as an InputSlotView part
    parts.emplace_back(InputSlotView{slot_index});

    // Update the position
    pos = match.position() + match.length();

    // Move to the next match
    ++it;
  }

  // Add any remaining text as a string part
  if (pos < text.length()) {
    parts.emplace_back(text.substr(pos));
  }
}

void BlockView::draw() {
  auto before_draw = ImGui::GetCursorScreenPos();

  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(40, 40, 40, 255));
  ImGui::BeginChild("", ImVec2(0, 0),
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
    ImGui::PushID(0);  // TODO: FIND A BETTER SOLUTION THAT DOING AN IMPOSSIBLE
                       // POINTER, THIS IS SO COUPLED
    draw();  // draw the same thing over, bro this is genius xD. I no longer
             // think so xD
    ImGui::PopID();
    ImGui::EndDragDropSource();
  }
  ImGui::PopStyleVar(2);
}

void BlockView::draw_parts() {
  for (int i = 0; i < _parts.size(); i++) {
    if (auto view = std::get_if<InputSlotView>(&_parts[i])) {
      std::shared_ptr<model::InputSlotInstance> slot =
          _block_instance->inputs()[view->index];

      if (slot->has_block()) {
        // TODO: Handle connected block
        // This would involve drawing the connected block or some representation
        // of it
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "[Connected Block]");
      } else {
        // Get the current value from the slot
        const model::Value& value = slot->value();

        ImGui::PushID(slot.get());
        ImGui::PushStyleVar(
            ImGuiStyleVar_FramePadding,
            ImVec2(DIRECT_VALUE_PADDING.first, DIRECT_VALUE_PADDING.second));

        // Handle the value based on its type
        if (value.type() == model::ValueType::NUMBER) {
          // For number values
          int num_value = int(value);

          std::string text_value = std::to_string(num_value);

          ImGui::SetNextItemWidth(ImGui::CalcTextSize(text_value.c_str()).x +
                                  10.f);
          if (ImGui::InputText("", &text_value,
                               ImGuiInputTextFlags_CharsDecimal)) {
            // Update the value if it changed
            model::Value new_value(model::ValueType::NUMBER);
            new_value.set(text_value.empty() ? 0 : std::stoi(text_value));
            slot->set_value(new_value);
          }
        } else if (value.type() == model::ValueType::TEXT) {
          // For text values
          std::string text_value = std::string(value);

          ImGui::SetNextItemWidth(ImGui::CalcTextSize(text_value.c_str()).x +
                                  10.f);
          if (ImGui::InputText("", &text_value)) {
            // Update the value if it changed
            model::Value new_value(model::ValueType::TEXT);
            new_value.set(text_value);
            slot->set_value(new_value);
          }
        }

        ImGui::PopStyleVar();
        ImGui::PopID();
      }
    } else if (auto* text = std::get_if<std::string>(&_parts[i])) {
      ImGui::AlignTextToFramePadding();
      ImGui::Text("%s", text->c_str());
    }

    ImGui::SameLine();
  }
}
}  // namespace ui
