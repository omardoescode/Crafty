#include "input_slot_instance.h"
#include "value.h"

namespace model {
InputSlotInstance::InputSlotInstance(const InputSlotDef& slot_def)
    : _slot_def(slot_def),
      _value(Value(slot_def.default_value())),
      _block_connected(false) {}

void InputSlotInstance::set_value(Value value) { _value = value; }

void InputSlotInstance::connect_block(std::shared_ptr<BlockInstance> instance) {
  _block_connected = true;
  _block = instance;
}

void InputSlotInstance::disconnect_block() {
  _block_connected = false;
  _block = nullptr;
}

bool InputSlotInstance::has_block() const { return _block_connected; }

const Value& InputSlotInstance::value() const { return _value; }

const InputSlotDef& InputSlotInstance::def() const { return _slot_def; }

const std::shared_ptr<BlockInstance> InputSlotInstance::block() const {
  return _block;
}
}  // namespace model
