#pragma once

#include <memory>
#include "block/BlockInstance.h"
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
  BlockView(UIOptions&, std::shared_ptr<model::BlockInstance>);
  void draw();

private:
  static constexpr float ROUNDING = 2.0f;
  static constexpr float BLOCK_WIDTH = 120.0f;
  static constexpr float BLOCK_HEIGHT = 40.0f;
  static constexpr float NOTCH_SIZE = 10.0f;  // Size of top/bottom notches
  static constexpr ImVec2 PADDING = {8, 8};   // Size of top/bottom notches
private:
  void draw_hat_block(ImVec2 pos);
  void draw_action_block(ImVec2 pos);

private:
  std::shared_ptr<model::BlockInstance> _block_instance;
  UIOptions& _options;
};
}  // namespace ui
