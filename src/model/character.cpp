#include "character.h"
#include <cassert>

namespace model {

Character::Character(const CharacterID& id) : _id(id) {}
void Character::add_texture(std::shared_ptr<Asset> asset, int pos) {
  if (pos == -1) pos = _textures.size() - 1;
  assert(pos >= 0 && pos < _textures.size() &&
         "Invalid position to insert into");
  _textures.insert(pos + _textures.begin(), asset);
}

void Character::remove_texture(int pos) {
  assert(pos >= 0 && pos < _textures.size() && "Invalid position to erase");
  _textures.erase(pos + _textures.begin());
}
bool Character::no_textures() const { return _textures.empty(); }

void Character::add_script(std::shared_ptr<Script> script) {
  assert(script && "Script is nullptr");
  assert(_scripts.count(script->id()) == 0 && "Duplicate Script ID detected");
  _scripts[script->id()] = (script);
}
void Character::remove_script(const Script::ScriptID& id) {
  assert(_scripts.count(id) == 1 && "Script doesn't exist");
  _scripts.erase(id);
}

std::pair<float, float> Character::pos() const { return _pos; }
void Character::set_pos(std::pair<float, float> new_pos) { _pos = new_pos; }

unsigned Character::current_texture_idx() const { return _current_texture_idx; }

std::shared_ptr<Asset> Character::current_texture() const {
  return _textures[_current_texture_idx];
}

void Character::set_current_texture_idx(int new_val) {
  _current_texture_idx = new_val;
}

void Character::next_texture() {
  _current_texture_idx++, _current_texture_idx %= _textures.size();
}
const std::string& Character::id() const { return _id; }
}  // namespace model
