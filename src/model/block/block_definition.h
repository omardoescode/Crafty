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
    BLOCKDEF_STARTER = 2,
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

  /**
   * @brief retrieve the unique id of the definition
   * @return returns the data id
   */
  const std::string& data_id() const;

  /**
   * @brief Check if the defintion is of a starter block
   * @return returns `true` if of a starter block
   */
  bool is_starter() const;

private:
  std::string _name;
  std::string _category;
  std::vector<InputSlotDef> _inputs;
  OutputSlot _output;
  std::string _data_id;
  bool _has_body;
  bool _is_starter;
};

/**
 * @brief A type alias for a shared pointer to a **constant** block definitions
 */
typedef std::shared_ptr<const BlockDefinition> BlockDefPtr;
};  // namespace model
