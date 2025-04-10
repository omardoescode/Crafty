#include "block_instance.h"
#include <cassert>
#include "block/block_instance.h"
#include "script.h"
#include "serialization/serializable.h"

namespace model {
BlockInstance::BlockInstance(IDPtr id, BlockDefPtr def, bool serialize)
    : Serializable(id, serialize), _def(def) {}

std::shared_ptr<const BlockDefinition> BlockInstance::def() const {
  return _def;
}

bool BlockInstance::has_body() { return _body == nullptr; }

IDPtr BlockInstance::body() {
  assert(has_body());
  return _body;
}

const std::vector<IDWPtr>& BlockInstance::inputs() const { return _inputs; }
}  // namespace model
