#include "project.h"
#include <cassert>

namespace model {
Project::Project(const ProjectID& id, const std::string& title)
    : _id(id), _title(title) {}
Project::Project(const ProjectID& id, std::string&& title)
    : _id(id), _title(std::move(title)) {}

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

const Project::ProjectID& Project::id() const { return _id; }
}  // namespace model
