#pragma once

#include <string>
#include <variant>
#include "block/value_type.h"
#include "error/type_mismatch.h"

namespace model {
/**
 * @breif A value used in input slot as a direct value
 * @invariant A value's type never changes
 */
class Value {
public:
  /**
   * @brief A type alias for the storage type used to store data
   */
  using StorageType = std::variant<int, std::string>;

  /*
   * @brief explicit constructor with rvalue
   */
  Value(ValueType);

  // copy
  Value(const Value&);
  Value& operator=(const Value&);

  /**
   * @brief move constructor
   */
  Value(Value&&);
  Value& operator=(Value&&);

  template <typename T>
  void set(T value) {
    // compile type check
    if (!is_compatible_type<T>())
      throw error::TypeMismatchError("Type mismatch in Value::set");
    _value = value;
  }

  /**
   * @brief Getter for type
   */
  ValueType type() const;

  operator int() const {
    if (_type != ValueType::NUMBER)
      throw error::TypeMismatchError("Expected NUMBER type");
    return std::get<int>(_value);
  }

  operator std::string() const {
    if (_type != ValueType::TEXT)
      throw error::TypeMismatchError("Expected NUMBER type");
    return std::get<std::string>(_value);
  }

private:
  template <typename T>
  constexpr bool is_compatible_type() const {
    switch (_type) {
      case ValueType::NUMBER:
        return std::is_same_v<T, int>;
      case ValueType::TEXT:
        return std::is_same_v<T, std::string> || std::is_same_v<T, const char*>;
      default:
        return false;
    }
  }

  StorageType _value;
  ValueType _type;
};
}  // namespace model
