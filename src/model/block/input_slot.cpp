#include "input_slot.h"

namespace model {
InputSlotDef::InputSlotDef(std::string&& label, Value default_value)
    : _label(std::move(label)), _default_value(Value(default_value)) {}

const std::string& InputSlotDef::label() const { return _label; }
const Value InputSlotDef::default_value() const { return _default_value; }
}  // namespace model
