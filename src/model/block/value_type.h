#pragma once
#include <string>

namespace model {
/**
 * @brief represents the actual value that the input slot store and inputted by
 * the user
 */
enum class ValueType { NUMBER, TEXT, VOID };

/**
 * @brief Parse a string type and convert it to `SlotType` enum
 * @param type The type in string format
 * @return SlotType corresponding to type
 */
ValueType to_value_type(std::string type);
}  // namespace model
