#include "value.h"
#include "block/value_type.h"
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
    case ValueType::VOID:  // do nothing
      break;
    default:
      throw model_logger().error("Failed to initilialize value");
  }
}

ValueType Value::type() const { return _type; }

Value::Value(const Value& rhs) : _type(rhs._type), _value(rhs._value) {}

Value& Value::operator=(const Value& rhs) {
  _type = rhs._type;
  _value = rhs._value;
  return *this;
}
Value::Value(Value&& rhs) : _type(rhs._type), _value(std::move(rhs._value)) {}

Value& Value::operator=(Value&& rhs) {
  _type = rhs._type;
  _value = std::move(rhs._value);
  return *this;
}
}  // namespace model
