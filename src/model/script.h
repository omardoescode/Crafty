#pragma once
#include <memory>
#include <vector>
#include "serialization/serializable.h"

namespace model {
class BlockInstance;

class Script : public Serializable {
public:
  Script(float x = 0, float y = 0);

  /**
   * Add a block instance to the script
   * @return the new script position. -1 if the instance is not added because
   * the instance is of a starter definition in a position not equal to 0
   * because starters cannot be inserted except at the beginning
   */
  int add_block_instance(std::shared_ptr<BlockInstance> instance, int pos = -1);
  void remove_block_instance(std::shared_ptr<BlockInstance> instance);
  bool has_block_instances() const;

  const std::pair<float, float> pos() const;

  const std::vector<std::shared_ptr<BlockInstance>>& blocks() const;

private:
  std::vector<std::shared_ptr<BlockInstance>> _blocks;
  std::pair<float, float> _pos;
};
}  // namespace model
