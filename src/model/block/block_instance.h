#pragma once

#include <memory>
#include <vector>
#include "asset.h"
#include "block/block_definition.h"
#include "block/block_library.h"
#include "serialization/serializable.h"

namespace model {
/**
 * @brief a block in a script that is executable when running the game
 * A block instanceis identified by a unique ID.
 * An instance with ID "" is called "dummy_instance" used for showing blocks
 * views in the block editor section without including them in a script
 */
class BlockInstance : public Serializable {
public:
  BlockInstance(IDPtr id, BlockDefPtr def, bool serialize = true);

  std::shared_ptr<const BlockDefinition> def() const;

  bool has_body();
  IDPtr body();
  const std::vector<IDWPtr>& inputs() const;

private:
  std::shared_ptr<const BlockDefinition> _def;
  std::vector<IDWPtr> _inputs;
  IDPtr _body;
};
}  // namespace model
