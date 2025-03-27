#include "character.h"
#include <algorithm>
#include <cassert>
#include "project.h"
#include "utils/ID_manager.h"
#include "utils/serializable.h"

namespace model {

Character::Character(const std::string& id, Project& project, float x, float y)
    : Serializable(id, project), _pos(x, y) {}
void Character::add_sprite(const IDManager::IDType& id, int pos) {
  assert(project_.asset_store().has_entity(id) &&
         "Asset store doesn't have this ID");
  if (pos < 0 || pos > _sprites.size()) pos = _sprites.size();
  _sprites.insert(_sprites.begin() + pos, id);
}

void Character::remove_sprite(const IDManager::IDType& id) {
  auto it = std::remove(_sprites.begin(), _sprites.end(), id);
  assert(it != _sprites.end());
  _sprites.erase(it, _sprites.end());
}
bool Character::has_sprites() const { return !_sprites.empty(); }

void Character::add_script(const IDManager::IDType& id, int pos) {
  assert(project_.script_store().has_entity(id) &&
         "Script store doesn't have this id");
  if (pos < 0 || pos > _scripts.size()) pos = _scripts.size();
  _scripts.insert(_scripts.begin() + pos, id);
}

void Character::remove_script(const IDManager::IDType& id) {
  auto it = std::remove(_scripts.begin(), _scripts.end(), id);
  assert(it != _sprites.end());
  _scripts.erase(it, _scripts.end());
}

bool Character::has_scripts() const { return !_scripts.empty(); }

std::pair<float, float> Character::pos() const { return _pos; }
void Character::set_pos(std::pair<float, float> new_pos) { _pos = new_pos; }

unsigned Character::current_texture_idx() const { return _current_texture_idx; }

const IDManager::IDType& Character::current_texture() const {
  return _sprites[_current_texture_idx];
}

void Character::set_current_texture_idx(size_t new_val) {
  assert(new_val >= 0 && new_val < _sprites.size() &&
         "Invalid value for a texture index");
  _current_texture_idx = new_val;
}

void Character::next_texture() {
  if (!has_sprites()) return;  // Guard against mod of 0
  _current_texture_idx++, _current_texture_idx %= _sprites.size();
}

const std::vector<IDManager::IDType>& Character::scripts() const {
  return _scripts;
}
const std::vector<IDManager::IDType>& Character::textures() const {
  return _sprites;
}
}  // namespace model
