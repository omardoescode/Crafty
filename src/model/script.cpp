#include "script.h"
#include <cassert>
#include "character.h"

namespace model {
Script::Script(const ScriptID& id, std::shared_ptr<Character> character,
               std::shared_ptr<BlockInstance> initial_block)
    : _id(id), _character(character) {
  add_block(initial_block);
}

void Script::add_block(std::shared_ptr<BlockInstance> block, int pos) {
  if (pos == -1) pos = _blocks.size() - 1;
  assert(pos >= 0 && pos < _blocks.size() && "Invalid position to insert into");
  _blocks.insert(_blocks.begin() + pos, block);
}

void Script::remove_block(int pos) {
  assert(pos >= 0 && pos < _blocks.size() && "Invalid Position to erase");
  _blocks.erase(_blocks.begin() + pos);
}
bool Script::empty_script() const { return _blocks.empty(); }

const Script::ScriptID& Script::id() const { return _id; }

const std::vector<std::shared_ptr<BlockInstance>>& Script::blocks() const {
  return _blocks;
}
std::shared_ptr<Character> Script::character() { return _character; }
}  // namespace model
