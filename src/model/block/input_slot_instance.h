#pragma once
#include "block/input_slot.h"
#include "identity/id.h"

namespace model {

/**
 * @brief A type alias to the input slot values in a block instance
 * @note An input slot value may have either a direct value or the output of
 * another block instance
 */
class InputSlotInstance {
public:
  /**
   * @brief Constructor
   * @param slot_def The input slot definition of this instance
   */
  InputSlotInstance(const InputSlotDef& slot_def);

  void set_value(Value value);
  void connect_block(IDPtr id);
  void disconnect_block();

  bool has_block() const;

  /**
   * @brief Get the current value of this input slot
   * @return The current value
   */
  const Value& value() const;

  const InputSlotDef& def() const;

  const IDPtr block_id() const;

private:
  Value _value;
  const InputSlotDef& _slot_def;
  bool _block_connected;
  IDPtr _block_id;
};
}  // namespace model
