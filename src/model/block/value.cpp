#include "value.h"
#include "model_logger.h"

namespace model {
Value::Value(ValueType type) : _type(type) {
  switch (_type) {
    case ValueType::NUMBER:
      _value = 0;
      break;
    case ValueType::TEXT:
      _value = "";
      break;
    default:
      model_logger().error("Failed to initialized value");
  }
}

ValueType Value::type() const { return _type; }
}  // namespace model
