#include "value_type.h"
#include <format>
#include <string>
#include "error/type_mismatch.h"
namespace model {
ValueType to_value_type(std::string type) {
  if (type == "Number")
    return ValueType::NUMBER;
  else if (type == "String")
    return ValueType::TEXT;
  else if (type == "Void")
    return ValueType::VOID;
  else
    throw error::TypeMismatchError(std::format("Invalid Type {}", type));
}
}  // namespace model
