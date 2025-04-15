#pragma once
#include <string>
#include "block/value.h"
namespace model {
class InputSlotDef {
public:
  /**
   * @brief Constructor
   * @param label Label used by the block instance
   * @param default_value The default value for this slot
   */
  InputSlotDef(std::string&& label, Value default_value);

  /**
   * @breif Getter for label
   * @return the label
   */
  const std::string& label() const;

  /**
   * @breif Getter for default Value
   * @return Default value
   */
  const Value default_value() const;

private:
  std::string _label;
  Value _default_value;
};
}  // namespace model
