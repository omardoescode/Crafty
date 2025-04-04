#include "block_instance.h"
#include <cassert>
#include "block/block_instance.h"
#include "script.h"
#include "utils/serializable.h"

namespace model {
BlockInstance::BlockInstance(const IDManager::IDType& id, Project& project,
                             std::shared_ptr<const BlockDefinition> def)
    : Serializable(id, project), _def(def) {}

BlockInstance::BlockInstance(IDManager::IDType&& id, Project& project,
                             std::shared_ptr<const BlockDefinition> def)
    : Serializable(id, project), _def(def) {}

bool BlockInstance::is_dummy_instance() const {
  return DUMMY_INSTANCE_ID == id();
}

std::shared_ptr<const BlockDefinition> BlockInstance::def() const {
  return _def;
}

bool BlockInstance::has_body() { return _body.empty(); }
IDManager::IDType BlockInstance::body() {
  assert(has_body());
  return _body;
}
}  // namespace model
