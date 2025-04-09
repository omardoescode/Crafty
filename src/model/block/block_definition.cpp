#include "block_definition.h"
#include <stdexcept>

namespace model {
BlockDefinition::BlockDefinition(const std::string& name,
                                 const std::string& category,
                                 std::vector<InputSlot>&& inputs,
                                 OutputSlot output_slot, int options)
    : _name(name), _category(category), _inputs(inputs), _output(output_slot) {
  if (options & BLOCKDEF_HASBODY) _has_body = true;
}

const std::string& BlockDefinition::name() const { return _name; }
const std::string& BlockDefinition::category() const { return _category; }
const std::vector<InputSlot>& BlockDefinition::inputs() const {
  return _inputs;
}
const OutputSlot& BlockDefinition::output() const { return _output; }

SlotType parse_type(std::string name) {
  if (name == "Exec")
    return SlotType::Exec;
  else if (name == "Number")
    return SlotType::Number;
  else if (name == "Boolean")
    return SlotType::Boolean;
  else if (name == "String")
    return SlotType::String;
  else
    throw std::runtime_error("Invalid type");
}
}  // namespace model
