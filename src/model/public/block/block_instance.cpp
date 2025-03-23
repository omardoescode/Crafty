#include "block_instance.h"
#include "block/block_instance.h"
#include "script.h"
#include "utils/serializable.h"

namespace model {
BlockInstance::BlockInstance(const IDManager::IDType& id, Project& project,
                             std::shared_ptr<const BlockDefinition> def,
                             float x, float y)
    : Serializable(id, project), _def(def), _x(x), _y(y) {}

BlockInstance::BlockInstance(IDManager::IDType&& id, Project& project,
                             std::shared_ptr<const BlockDefinition> def,
                             float x, float y)
    : Serializable(id, project), _def(def), _x(x), _y(y) {}

bool BlockInstance::is_dummy_instance() const {
  return DUMMY_INSTANCE_ID == id();
}
}  // namespace model
