#pragma once

#include <string>
#include <vector>

namespace model {
enum class SlotType { Exec, Number, Boolean, String, Any };

struct InputSlot {
  std::string label;
  SlotType type;
  std::string default_value;
};

struct OutputSlot {
  SlotType type;
};

typedef std::string BlockDefinitionID;
// Holds information about the block definitions retrieved from the filesystem
// via BlockLibrary
struct BlockDefinition {
  BlockDefinitionID id;
  std::string name;
  std::string category;
  OutputSlot output;
  bool has_body;
};
};  // namespace model
