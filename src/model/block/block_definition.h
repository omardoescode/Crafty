#pragma once

#include <string>
#include <vector>
#include "block_library.h"

namespace model {

class BlockDefinition {
public:
  enum options {
    BLOCKDEF_HASBODY = 1,
  };

  enum class SlotType { Exec, Number, Boolean, String, Any };

  struct InputSlot {
    std::string label;
    SlotType type;
    std::string default_value;
  };

  struct OutputSlot {
    SlotType type;
  };

public:
  BlockDefinition(const BlockLibrary::BlockDefIDType& id,
                  const std::string& name, const std::string& category,
                  std::vector<InputSlot>&& inputs, OutputSlot output_slot,
                  int options = 0);

  const BlockLibrary::BlockDefIDType& id() const;
  const std::string& name() const;
  const std::string& category() const;
  const std::vector<InputSlot>& inputs() const;
  const OutputSlot& output() const;

private:
  BlockLibrary::BlockDefIDType _id;
  std::string _name;
  std::string _category;
  std::vector<InputSlot> _inputs;
  OutputSlot _output;
  bool _has_body;
};
};  // namespace model
