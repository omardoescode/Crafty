#include "input_slot_instance.h"
#include "value.h"

namespace model {
InputSlotInstance::InputSlotInstance(const InputSlotDef& slot_def)
    : _slot_def(slot_def),
      _value(Value(slot_def.default_value())),
      _block_connected(false) {}

void InputSlotInstance::set_value(Value value) { _value = value; }

void InputSlotInstance::connect_block(IDPtr id) {
  _block_connected = true;
  _id = id;
}

void InputSlotInstance::disconnect_block() {
  _block_connected = false;
  _id = nullptr;
}

bool InputSlotInstance::has_block() const { return _block_connected; }

const Value& InputSlotInstance::value() const { return _value; }
}  // namespace model
