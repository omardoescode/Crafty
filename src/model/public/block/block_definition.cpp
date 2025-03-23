#include "block_definition.h"

namespace model {
BlockDefinition::BlockDefinition(const BlockLibrary::BlockDefIDType& id,
                                 const std::string& name,
                                 const std::string& category,
                                 std::vector<InputSlot>&& inputs,
                                 OutputSlot output_slot, int options)
    : _id(id),
      _name(name),
      _category(category),
      _inputs(inputs),
      _output(output_slot) {
  if (options & BLOCKDEF_HASBODY) _has_body = true;
}

const BlockLibrary::BlockDefIDType& BlockDefinition::id() const { return _id; }
const std::string& BlockDefinition::name() const { return _name; }
const std::string& BlockDefinition::category() const { return _category; }
const std::vector<BlockDefinition::InputSlot>& BlockDefinition::inputs() {
  return _inputs;
}
const BlockDefinition::OutputSlot& BlockDefinition::output() { return _output; }
}  // namespace model
