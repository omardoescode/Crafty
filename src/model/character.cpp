#include "character.h"
#include <algorithm>
#include <cassert>
#include "model_logger.h"

namespace model {

Character::Character(const std::string& name, int x, int y, float width)
    : _pos(x, y),
      _current_texture_idx(0),
      _name(name),
      _width(width),
      _rotation(0) {}
Character::Character(std::string&& name, int x, int y, float width)
    : _pos(x, y),
      _current_texture_idx(0),
      _name(std::move(name)),
      _width(width),
      _rotation(0) {}

void Character::add_sprite(std::shared_ptr<Asset> sprite, int pos) {
  if (pos < 0 || pos > _sprites.size()) pos = _sprites.size();
  _sprites.insert(_sprites.begin() + pos, sprite);
}

void Character::remove_sprite(std::shared_ptr<Asset> asset) {
  assert(_sprites.size() > 1 &&
         "Cannot remove the last asset. Remove the character instead");

  auto it = std::find(_sprites.begin(), _sprites.end(), asset);
  if (it != _sprites.end()) {
    int idx = it - _sprites.begin();
    bool is_current = idx == _current_texture_idx;
    _sprites.erase(it);

    if (is_current) _current_texture_idx = 0;  // Set to the first one
  }
}
bool Character::has_sprites() const { return !_sprites.empty(); }

void Character::add_script(std::shared_ptr<Script> script, int pos) {
  if (pos < 0 || pos > _scripts.size()) pos = _scripts.size();
  _scripts.insert(_scripts.begin() + pos, script);
}

void Character::remove_script(std::shared_ptr<Script> script) {
  model_logger().info("Removing A script");
  _scripts.erase(std::remove(_scripts.begin(), _scripts.end(), script),
                 _scripts.end());
}

bool Character::has_scripts() const { return !_scripts.empty(); }

std::pair<int, int> Character::pos() const { return _pos; }
void Character::set_pos(std::pair<int, int> pos) { _pos = pos; }

unsigned Character::current_spirte_idx() const { return _current_texture_idx; }

std::shared_ptr<Asset> Character::current_sprite() const {
  if (_sprites.empty()) return nullptr;
  auto result = _sprites[_current_texture_idx];

  return result;
}

void Character::set_current_sprite_idx(size_t new_val) {
  assert(new_val >= 0 && new_val < _sprites.size() &&
         "Invalid value for a texture index");
  _current_texture_idx = new_val;
}

void Character::next_sprite() {
  // Note: Assuming the invariant that there be always be sprites at any moment,
  // there mustn't be any problem with mode 0 (_sprites.size=0)
  _current_texture_idx++, _current_texture_idx %= _sprites.size();
}

const std::vector<std::shared_ptr<Script>>& Character::scripts() const {
  return _scripts;
}
const std::vector<std::shared_ptr<Asset>>& Character::sprites() const {
  return _sprites;
}
const std::string Character::name() const { return _name; }
void Character::set_name(const std::string& name) { _name = name; }
void Character::set_name(std::string&& name) { _name = std::move(name); }

const float Character::width() const { return _width; }
void Character::set_width(float width) { _width = width; }

int Character::rotation() const { return _rotation; }

void Character::set_rotation(int new_rotation) {
  _rotation = new_rotation % 360;
  if (_rotation < 0) _rotation += 360;
}
}  // namespace model
