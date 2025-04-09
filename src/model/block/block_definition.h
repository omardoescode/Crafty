#pragma once

#include <memory>
#include <string>
#include <vector>

namespace model {

enum class SlotType { Exec, Number, Boolean, String };

/**
 * @brief Parse a string type and convert it to `SlotType` enum
 * @param type The type in string format
 * @return a SlotType
 */
SlotType parse_type(std::string type);

struct InputSlot {
  std::string label;
  SlotType type;
  std::string default_value;
};

struct OutputSlot {
  SlotType type;
};

class BlockDefinition {
public:
  enum options {
    BLOCKDEF_HASBODY = 1,
  };

public:
  BlockDefinition(const std::string& name, const std::string& category,
                  std::vector<InputSlot>&& inputs, OutputSlot output_slot,
                  int options = 0);

  const std::string& name() const;
  const std::string& category() const;
  const std::vector<InputSlot>& inputs() const;
  const OutputSlot& output() const;

private:
  std::string _name;
  std::string _category;
  std::vector<InputSlot> _inputs;
  OutputSlot _output;
  bool _has_body;
};

/**
 * @brief A type alias for a shared pointer to a **constant** block definitions
 */
typedef std::shared_ptr<const BlockDefinition> BlockDefPtr;
};  // namespace model
