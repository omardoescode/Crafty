#pragma once
#include <memory>
#include <vector>
#include "project.h"
#include "serialization/serializable.h"

namespace model {
class Project;
class Character;
class BlockInstance;

class Script : public Serializable {
public:
  Script(const IDPtr id, std::shared_ptr<Character> character, float x, float y,
         bool serialize = true);

  /**
   * Add a block instance to the script
   * @return the new script position
   */
  int add_block_instance(ProjectPtr project, IDPtr id, int pos = -1);
  void remove_block_instance(IDPtr id);
  bool has_block_instances() const;

  /**
   * @brief Return the character that this script is executing on
   */
  std::shared_ptr<Character> character();

  const std::pair<float, float> pos() const;

  const std::vector<IDWPtr>& blocks() const;

private:
  std::vector<IDWPtr> _blocks;
  std::pair<float, float> _pos;
  std::shared_ptr<Character> _character;
};
}  // namespace model
