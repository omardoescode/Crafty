#include "script.h"
#include <algorithm>
#include <cassert>
#include "block/block_instance.h"
#include "character.h"
#include "model_logger.h"

namespace model {
Script::Script(float x, float y) : _pos(x, y) {}

int Script::add_block_instance(std::shared_ptr<BlockInstance> instance,
                               int pos) {
  pos = std::clamp(pos, 0, static_cast<int>(_blocks.size()));

  // We cannot add a starter except to the beginning
  if (instance->def()->is_starter() && pos != 0) return -1;

  _blocks.insert(_blocks.begin() + pos, instance);
  model_logger().info("Added a block instance to script");
  return pos;
}

void Script::remove_block_instance(std::shared_ptr<BlockInstance> instance) {
  _blocks.erase(std::remove(_blocks.begin(), _blocks.end(), instance),
                _blocks.end());
}
bool Script::has_block_instances() const { return !_blocks.empty(); }
const std::pair<float, float> Script::pos() const { return _pos; }

const std::vector<std::shared_ptr<BlockInstance>>& Script::blocks() const {
  return _blocks;
}
}  // namespace model
