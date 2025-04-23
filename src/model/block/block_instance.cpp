#include "block_instance.h"
#include <cassert>
#include <memory>
#include "block/block_instance.h"
#include "input_slot_instance.h"
#include "model_logger.h"
#include "script.h"

namespace model {
BlockInstance::BlockInstance(BlockDefPtr def) : _def(def) {
  // Initialize Input Instances
  for (const InputSlotDef& inp_def : def->inputs()) {
    _inputs.push_back(std::make_shared<InputSlotInstance>(inp_def));
  }
}

std::shared_ptr<const BlockDefinition> BlockInstance::def() const {
  return _def;
}

bool BlockInstance::has_body() const { return !!_body; }

std::shared_ptr<Script> BlockInstance::body() { return _body; }

const std::vector<std::shared_ptr<InputSlotInstance>>& BlockInstance::inputs()
    const {
  return _inputs;
}

void BlockInstance::set_input_slot_value(
    std::shared_ptr<InputSlotInstance> value, std::size_t idx) {
  if (idx < 0 || idx > _inputs.size())
    throw model_logger().error(
        "BlockInstance::set_input_slot_value - Invalid index={} given", idx);
  _inputs[idx] = value;
}
}  // namespace model
