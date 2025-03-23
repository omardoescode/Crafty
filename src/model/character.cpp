#include "character.h"
#include <algorithm>
#include <cassert>
#include "project.h"
#include "utils/ID_manager.h"
#include "utils/serializable.h"

namespace model {

Character::Character(const std::string& id, Project& project)
    : Serializable(id, project) {}
void Character::add_texture(const IDManager::IDType& id, int pos) {
  assert(project_.asset_store().has_entity(id) &&
         "Asset store doesn't have this ID");
  if (pos < 0 || pos > _textures.size()) pos = _textures.size();
  _textures.insert(_textures.begin() + pos, id);
}

void Character::remove_texture(const IDManager::IDType& id) {
  auto it = std::remove(_textures.begin(), _textures.end(), id);
  assert(it != _textures.end());
  _textures.erase(it, _textures.end());
}
bool Character::has_textures() const { return !_textures.empty(); }

void Character::add_script(const IDManager::IDType& id, int pos) {
  assert(project_.script_store().has_entity(id) &&
         "Script store doesn't have this id");
  if (pos < 0 || pos > _scripts.size()) pos = _scripts.size();
  _scripts.insert(_scripts.begin() + pos, id);
}

void Character::remove_script(const IDManager::IDType& id) {
  auto it = std::remove(_scripts.begin(), _scripts.end(), id);
  assert(it != _textures.end());
  _scripts.erase(it, _scripts.end());
}

bool Character::has_scripts() const { return !_scripts.empty(); }

std::pair<float, float> Character::pos() const { return _pos; }
void Character::set_pos(std::pair<float, float> new_pos) { _pos = new_pos; }

unsigned Character::current_texture_idx() const { return _current_texture_idx; }

const IDManager::IDType& Character::current_texture() const {
  return _textures[_current_texture_idx];
}

void Character::set_current_texture_idx(size_t new_val) {
  assert(new_val >= 0 && new_val < _textures.size() &&
         "Invalid value for a texture index");
  _current_texture_idx = new_val;
}

void Character::next_texture() {
  if (!has_textures()) return;  // Guard against mod of 0
  _current_texture_idx++, _current_texture_idx %= _textures.size();
}

const std::vector<IDManager::IDType>& Character::scripts() const {
  return _scripts;
}
const std::vector<IDManager::IDType>& Character::textures() const {
  return _textures;
}
}  // namespace model
