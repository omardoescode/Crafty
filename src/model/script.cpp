#include "script.h"
#include <algorithm>
#include <cassert>
#include "character.h"
#include "identity/id.h"
#include "project.h"

namespace model {
Script::Script(IDPtr id, std::shared_ptr<Character> character, float x, float y,
               bool serialize)
    : Serializable(id, serialize), _character(character), _pos(x, y) {}

int Script::add_block_instance(IDPtr id, int pos) {
  pos = std::clamp(pos, 0, static_cast<int>(_blocks.size()));
  _blocks.insert(_blocks.begin() + pos, id);
  return pos;
}

void Script::remove_block_instance(IDPtr id) { remove_weak_ptr(_blocks, id); }
bool Script::has_block_instances() const { return !_blocks.empty(); }
const std::pair<float, float> Script::pos() const { return _pos; }

const std::vector<IDWPtr>& Script::blocks() const { return _blocks; }

std::shared_ptr<Character> Script::character() { return _character; }
}  // namespace model
