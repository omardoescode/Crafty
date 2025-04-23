#pragma once
#include "block/block_instance.h"
#include "block/input_slot.h"

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
  void connect_block(std::shared_ptr<BlockInstance> instance);
  void disconnect_block();

  bool has_block() const;

  /**
   * @brief Get the current value of this input slot
   * @return The current value
   */
  const Value& value() const;

  const InputSlotDef& def() const;

  const std::shared_ptr<BlockInstance> block() const;

private:
  Value _value;
  const InputSlotDef& _slot_def;
  bool _block_connected;
  std::shared_ptr<BlockInstance> _block;
};
}  // namespace model
