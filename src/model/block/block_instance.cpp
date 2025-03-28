#include "block_instance.h"
#include <cassert>
#include "block/block_instance.h"
#include "script.h"
#include "utils/serializable.h"

namespace model {
BlockInstance::BlockInstance(const IDManager::IDType& id, Project& project,
                             std::shared_ptr<const BlockDefinition> def,
                             float x, float y)
    : Serializable(id, project), _def(def), _pos(x, y) {}

BlockInstance::BlockInstance(IDManager::IDType&& id, Project& project,
                             std::shared_ptr<const BlockDefinition> def,
                             float x, float y)
    : Serializable(id, project), _def(def), _pos(x, y) {}

bool BlockInstance::is_dummy_instance() const {
  return DUMMY_INSTANCE_ID == id();
}

std::shared_ptr<const BlockDefinition> BlockInstance::def() const {
  return _def;
}
float BlockInstance::x() const { return _pos.first; }
float BlockInstance::y() const { return _pos.second; }

bool BlockInstance::has_body() { return _body.empty(); }
IDManager::IDType BlockInstance::body() {
  assert(has_body());
  return _body;
}
}  // namespace model
