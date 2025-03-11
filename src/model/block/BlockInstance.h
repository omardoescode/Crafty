#pragma once

#include <list>
#include <string>
#include <vector>
#include "block/BlockDefinition.h"
namespace model {
// For IDS, define 0 as a special ID that stands for NULL IDs.
typedef std::string BlockInstanceID;
typedef std::string ScriptID;
constexpr std::string DUMMY_INSTANCE_ID = "";

/**
 * @brief A block instance is identified by a unique ID.
 * An instance with ID "" is called "dummy_instance" used for showing blocks
 * views in the block editor section without including them in a script
 */
struct BlockInstance {
  BlockInstanceID id;  // a unique identifier
  BlockDefinitionID definition_id;
  float x, y;
  std::vector<BlockInstanceID> inputs;
  ScriptID body;
};

/**
 * @brief A script is a collection of blocks that start with a starting block
 * such as "when signal received" or "when green flag clicked" A script is
 * identified by an ID, and and a character
 */
struct script {
  ScriptID id;
  std::list<BlockInstanceID> blocks;
  // Character character;
};

// Used for creating dummy instances, that is, instances that don't exist in the
// pool, and have an invalid ID of `DUMMY_INSTANCE_ID`
BlockInstance create_dummy_instance(BlockDefinitionID);

bool is_dummy_instance(BlockInstance);

}  // namespace model
