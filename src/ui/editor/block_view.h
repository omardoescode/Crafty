#pragma once

#include <cstddef>
#include <memory>
#include <variant>
#include "block/block_instance.h"
#include "imgui.h"
#include "ui_options.h"

namespace ui {
/**
 * A Block in UI layer is classified as
 * 1. Hat block: A block that is starting a script. Examples are: "When mouse
 * clicked"
 * 2. Action Block: A regular block that executes a command such as "move 10
 * steps"
 * 3. boolean blocks: A block that returns a boolean such as and block
 * 4. Number blocks: A block that returns a number such as + block
 * 5. Construct block: A block that has a body, such as "if "
 */
class BlockView {
public:
  constexpr static int DIRECT_VALUE_BUFFER_SIZE = 8;

public:
  BlockView(UIOptions&, std::shared_ptr<model::BlockInstance>, bool);
  void draw();

private:
  constexpr static std::pair<float, float> PADDING = {2.f, 2.f};
  constexpr static std::pair<float, float> DIRECT_VALUE_PADDING = {3.f, 3.f};
  constexpr static float SLOT_X_MARGIN = 3.f;
  constexpr static float MIN_VIEW_HEIGHT = 20.f;

  struct InputSlotView {
    size_t index;  // The corresponding index in the inputs
  };
  using BlockPart = std::variant<std::string, InputSlotView>;

  void eval_size();
  void draw_parts();
  void handle_drag();
  void parse_block_name();
  void handle_text(const std::string& text, std::vector<BlockPart>& parts);

  std::shared_ptr<model::BlockInstance> _block_instance;
  UIOptions& _options;
  std::vector<BlockPart> _parts;
  bool _draggable;
};
}  // namespace ui
