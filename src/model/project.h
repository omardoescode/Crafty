#pragma once

#include <map>

#include "character.h"
namespace model {
class Project {
public:
public:
  Project(const std::string& title);
  Project(std::string&& title);

  void add_character(std::shared_ptr<Character>);
  void remove_character(const Character::CharacterID&);
  const std::map<Character::CharacterID, std::shared_ptr<Character>>&
  characters();

private:
  std::map<Character::CharacterID, std::shared_ptr<Character>> _characters;
  std::string _title;
};
}  // namespace model
