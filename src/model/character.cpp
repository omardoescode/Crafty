#include "character.h"
#include <algorithm>
#include <cassert>
#include "identity/id.h"
#include "model_logger.h"
#include "project.h"

namespace model {

Character::Character(IDPtr id, int x, int y, float width, bool serialize)
    : Serializable(id, serialize),
      _pos(x, y),
      _current_texture_idx(0),
      _name(id->to_string()),
      _width(width),
      _rotation(0) {}

void Character::add_sprite(IDPtr id, int pos) {
  if (pos < 0 || pos > _sprites.size()) pos = _sprites.size();
  _sprites.insert(_sprites.begin() + pos, id);
}

void Character::remove_sprite(IDPtr id) { remove_weak_ptr(_sprites, id); }
bool Character::has_sprites() const { return !_sprites.empty(); }

void Character::add_script(IDPtr id, int pos) {
  if (pos < 0 || pos > _scripts.size()) pos = _scripts.size();
  _scripts.insert(_scripts.begin() + pos, id);
}

void Character::remove_script(IDPtr id) { remove_weak_ptr(_scripts, id); }

bool Character::has_scripts() const { return !_scripts.empty(); }

std::pair<int, int> Character::pos() const { return _pos; }
void Character::set_pos(std::pair<int, int> pos) { _pos = pos; }

unsigned Character::current_spirte_idx() const { return _current_texture_idx; }

IDPtr Character::current_sprite() const {
  if (_sprites.empty()) return nullptr;
  auto result_w = _sprites[_current_texture_idx];
  if (auto result = result_w.lock()) return result;
  throw model_logger().error(
      "There's no current implementation to changing current texture when a "
      "texture is removed");
}

void Character::set_current_sprite_idx(size_t new_val) {
  assert(new_val >= 0 && new_val < _sprites.size() &&
         "Invalid value for a texture index");
  _current_texture_idx = new_val;
}

void Character::next_sprite() {
  if (!has_sprites()) return;  // Guard against mod of 0
  _current_texture_idx++, _current_texture_idx %= _sprites.size();
}

const std::vector<IDWPtr>& Character::scripts() const { return _scripts; }
const std::vector<IDWPtr>& Character::sprites() const { return _sprites; }
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
