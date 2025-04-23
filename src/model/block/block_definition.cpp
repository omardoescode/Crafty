#include "block_definition.h"

namespace model {
BlockDefinition::BlockDefinition(std::string data_id, const std::string& name,
                                 const std::string& category,
                                 std::vector<InputSlotDef>&& inputs,
                                 OutputSlot output_slot, int options)
    : _id(data_id),
      _name(name),
      _category(category),
      _inputs(inputs),
      _output(output_slot) {
  if (options & BLOCKDEF_HASBODY) _has_body = true;
  if (options & BLOCKDEF_STARTER) _is_starter = true;
}

const std::string& BlockDefinition::name() const { return _name; }
const std::string& BlockDefinition::category() const { return _category; }
const std::vector<InputSlotDef>& BlockDefinition::inputs() const {
  return _inputs;
}
const OutputSlot& BlockDefinition::output() const { return _output; }
bool BlockDefinition::has_body() const { return _has_body; }

const std::string& BlockDefinition::id() const { return _id; }

bool BlockDefinition::is_starter() const { return _is_starter; }
}  // namespace model
