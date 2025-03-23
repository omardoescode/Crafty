#include "script.h"
#include <algorithm>
#include <cassert>
#include "project.h"
#include "utils/ID_manager.h"
#include "utils/serializable.h"

namespace model {
Script::Script(const IDManager::IDType& id, Project& project)
    : Serializable(id, project) {}

void Script::add_block_instance(const IDManager::IDType& id, int pos) {
  assert(project_.instances_store().has_entity(id));
  if (pos < 0 || pos > _blocks.size()) pos = _blocks.size();
  _blocks.insert(_blocks.begin() + pos, id);
}

void Script::remove_block_instance(const IDManager::IDType& id) {
  auto it = std::remove(_blocks.begin(), _blocks.end(), id);
  assert(it != _blocks.end());
  _blocks.erase(it, _blocks.end());
}
bool Script::has_block_instances() const { return !_blocks.empty(); }

const std::vector<IDManager::IDType>& Script::blocks() const { return _blocks; }
}  // namespace model
