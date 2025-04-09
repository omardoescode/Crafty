#include "block_instance.h"
#include <cassert>
#include "block/block_instance.h"
#include "script.h"

namespace model {
BlockInstance::BlockInstance(IDPtr id, Project& project,
                             std::shared_ptr<const BlockDefinition> def)
    : Serializable(id, project), _def(def) {}

std::shared_ptr<const BlockDefinition> BlockInstance::def() const {
  return _def;
}

bool BlockInstance::has_body() { return _body == nullptr; }

IDPtr BlockInstance::body() {
  assert(has_body());
  return _body;
}
}  // namespace model
