#pragma once
#include <memory>
#include <vector>
#include "identity/id/id.h"
#include "identity/serializable/serializable.h"

namespace model {
class Project;
class Character;
class BlockInstance;

class Script : public Serializable {
public:
  Script(const IDPtr id, Project& project, std::shared_ptr<Character> character,
         float x, float y);

  /**
   * Add a block instance to the script
   * @return the new script position
   */
  int add_block_instance(IDPtr id, int pos = -1);
  void remove_block_instance(IDPtr id);
  bool has_block_instances() const;

  /**
   * @brief Return the character that this script is executing on
   */
  std::shared_ptr<Character> character();

  const std::pair<float, float> pos() const;

  const std::vector<IDPtr>& blocks() const;

private:
  std::vector<IDPtr> _blocks;
  std::pair<float, float> _pos;
  std::shared_ptr<Character> _character;
};
}  // namespace model
