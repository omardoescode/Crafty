#include "project.h"
#include <cassert>

namespace model {
Project::Project(const std::string& title) : _title(title) {}
Project::Project(std::string&& title) : _title(std::move(title)) {}

void Project::add_character(std::shared_ptr<Character> character) {
  assert(character && "Character is nullptr");
  assert(_characters.count(character->id()) == 0 && "Character already exists");
  _characters[character->id()] = character;
}

void Project::remove_character(const Character::CharacterID& id) {
  assert(_characters.count(id) == 1 && "Character doesn't exist");
  _characters.erase(id);
}

const std::map<Character::CharacterID, std::shared_ptr<Character>>&
Project::characters() {
  return _characters;
}
}  // namespace model
