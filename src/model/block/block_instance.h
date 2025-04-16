#pragma once

#include <memory>
#include <vector>
#include "block/block_definition.h"
#include "block/block_library.h"
#include "block/input_slot_instance.h"
#include "serialization/serializable.h"

namespace model {
/**
 * @brief a block in a script that is executable when running the game
 * A block instanceis identified by a unique ID.
 */
class BlockInstance : public Serializable {
public:
  /**
   * @brief Constructor
   * @param id The id of this instance
   * @param def The definition of this block instance
   * @param serialize whether to serialize this or not
   */
  BlockInstance(IDPtr id, BlockDefPtr def, bool serialize = true);

  /**
   * @brief Getter for definition
   * @return A shared pointer to the definition
   */
  BlockDefPtr def() const;

  /**
   * @brief Check for existent body
   * @return `true` if there's a body
   * @note This checks for an existent body in general and not whether this
   * definiton accepts a body or not
   */
  bool has_body() const;
  /**
   * @brief Getter for body if the block instance accepts a body
   * @return a weak pointer to the body ID
   */
  IDWPtr body();

  /**
   * @brief Getter for inputs
   * @return a constant view of the inputs as a vector
   */
  const std::vector<std::shared_ptr<InputSlotInstance>>& inputs() const;

  /**
   * @brief Set the value of the input slot value at the specified index
   * @param value The value to be set
   * @param idx The index to set the value in
   */
  void set_input_slot_value(std::shared_ptr<InputSlotInstance> value,
                            std::size_t idx);

private:
  std::shared_ptr<const BlockDefinition> _def;
  std::vector<std::shared_ptr<InputSlotInstance>> _inputs;
  IDWPtr _body;
};
}  // namespace model
