#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "block/block_definition.h"
#include "block/block_library.h"
#include "utils/ID_manager.h"
#include "utils/serializable.h"

namespace model {
/**
 * @brief a block in a script that is executable when running the game
 * A block instanceis identified by a unique ID.
 * An instance with ID "" is called "dummy_instance" used for showing blocks
 * views in the block editor section without including them in a script
 */
class BlockInstance : public Serializable {
public:
  constexpr static std::string DUMMY_INSTANCE_ID = "";

public:
  BlockInstance(const IDManager::IDType&, Project& project,
                std::shared_ptr<const BlockDefinition>, float, float);
  BlockInstance(IDManager::IDType&&, Project& project,
                std::shared_ptr<const BlockDefinition>, float, float);

  bool is_dummy_instance() const;

  std::shared_ptr<const BlockDefinition> def() const;
  float x() const;
  float y() const;

  bool has_body();
  IDManager::IDType body();
  // TODO: Add metthod to control input slots
private:
  std::shared_ptr<const BlockDefinition> _def;
  std::pair<float, float> _pos;
  std::vector<IDManager::IDType> _inputs;
  // TODO: handle script body
  IDManager::IDType _body;
};
}  // namespace model
