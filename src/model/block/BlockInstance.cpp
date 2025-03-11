#include "BlockInstance.h"
namespace model {
BlockInstance create_dummy_instance(BlockDefinitionID def) {
  return BlockInstance{
      .id = DUMMY_INSTANCE_ID,  // invalid ID
      .definition_id = def,
      .x = -1,
      .y = -1  // invalid values
  };
}

bool is_dummy_instance(BlockInstance instance) {
  return instance.id == DUMMY_INSTANCE_ID;
}
}  // namespace model
