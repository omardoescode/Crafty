#pragma once

#include <memory>
#include <vector>
#include "block/block_definition.h"
#include "block/block_library.h"
#include "identity/serializable/serializable.h"

namespace model {
/**
 * @brief a block in a script that is executable when running the game
 * A block instanceis identified by a unique ID.
 * An instance with ID "" is called "dummy_instance" used for showing blocks
 * views in the block editor section without including them in a script
 */
class BlockInstance : public Serializable {
public:
  BlockInstance(IDPtr id, Project& project,
                std::shared_ptr<const BlockDefinition>);

  std::shared_ptr<const BlockDefinition> def() const;

  bool has_body();
  IDPtr body();
  // TODO: Add metthod to control input slots
private:
  std::shared_ptr<const BlockDefinition> _def;
  std::vector<IDPtr> _inputs;
  // TODO: handle script body
  IDPtr _body;
};
}  // namespace model
