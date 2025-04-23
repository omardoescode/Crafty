#pragma once

#include <memory>
#include <vector>
#include "character.h"

namespace model {
class Project {
public:
  Project(const std::string& name,
          std::vector<std::shared_ptr<Character>> character_list = {});

  const std::vector<std::shared_ptr<Character>>& characters() const;

  void add_character(std::shared_ptr<Character> chr);
  void remove_character(std::shared_ptr<Character> chr);

  const std::string& name() const;
  void set_name(const std::string& new_name);

  void mark_dirty();
  void mark_undirty();
  bool is_dirty() const;

private:
  std::vector<std::shared_ptr<Character>> _character_list;
  std::string _name;
  bool _is_dirty;
};

/**
 * @brief A type alias for a shared pointer to Project
 */
typedef std::shared_ptr<Project> ProjectPtr;
}  // namespace model
