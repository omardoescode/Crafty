#include "script.h"
#include <algorithm>
#include <cassert>
#include "character.h"
#include "identity/id/id.h"
#include "project.h"

namespace model {
Script::Script(IDPtr id, Project& project, std::shared_ptr<Character> character,
               float x, float y)
    : Serializable(id, project), _character(character), _pos(x, y) {}

int Script::add_block_instance(IDPtr id, int pos) {
  assert(project_.instances_store().has_entity(id));
  if (pos < 0 || pos > _blocks.size())
    _blocks.push_back(id), pos = _blocks.size() - 1;
  else
    _blocks.insert(_blocks.begin() + pos, id);
  return pos;
}

void Script::remove_block_instance(IDPtr id) {
  auto it = std::remove(_blocks.begin(), _blocks.end(), id);
  assert(it != _blocks.end());
  _blocks.erase(it, _blocks.end());
}
bool Script::has_block_instances() const { return !_blocks.empty(); }
const std::pair<float, float> Script::pos() const { return _pos; }

const std::vector<IDPtr>& Script::blocks() const { return _blocks; }

std::shared_ptr<Character> Script::character() { return _character; }
}  // namespace model
