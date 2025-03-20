#pragma once

#include <map>

#include "character.h"
namespace model {
class Project {
public:
  typedef std::string ProjectID;

public:
  Project(const ProjectID& id, const std::string& title);
  Project(const ProjectID& id, std::string&& title);

  void add_character(std::shared_ptr<Character>);
  void remove_character(const Character::CharacterID&);
  const std::map<Character::CharacterID, std::shared_ptr<Character>>&
  characters();

  const ProjectID& id() const;

private:
  std::map<Character::CharacterID, std::shared_ptr<Character>> _characters;
  std::string _title;
  ProjectID _id;
};
}  // namespace model
