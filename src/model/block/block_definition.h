#pragma once

#include <memory>
#include <string>
#include <vector>
#include "identity/identifiable.h"
#include "input_slot.h"

namespace model {

struct OutputSlot {
  ValueType type;
};

class BlockDefinition : public Identifiable {
public:
  /**
   * @brief Options for BlockDefinition Initialization
   */
  enum options {
    BLOCKDEF_DEFAULT = 0,
    BLOCKDEF_HASBODY = 1,
  };

  BlockDefinition(IDPtr id, std::string data_id, const std::string& name,
                  const std::string& category,
                  std::vector<InputSlotDef>&& inputs, OutputSlot output_slot,
                  int options = 0);

  /**
   * @brief Getter for name
   */
  const std::string& name() const;

  /**
   * @brief Getter for category
   */
  const std::string& category() const;

  /**
   * @brief Getter for input slots
   */
  const std::vector<InputSlotDef>& inputs() const;

  /**
   * @brief Getter for output slot
   */
  const OutputSlot& output() const;

  /**
   * @brief Check if block accepts a body
   */
  bool has_body() const;

private:
  std::string _name;
  std::string _category;
  std::vector<InputSlotDef> _inputs;
  OutputSlot _output;
  bool _has_body;
  std::string _data_id;
};

/**
 * @brief A type alias for a shared pointer to a **constant** block definitions
 */
typedef std::shared_ptr<const BlockDefinition> BlockDefPtr;
};  // namespace model
