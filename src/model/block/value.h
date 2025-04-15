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
  explicit Value(ValueType);

  template <typename T>
  T get() const {
    return std::get<T>(_value);
  }

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

private:
  template <typename T>
  constexpr bool is_compatible_type() const {
    switch (_type) {
      case ValueType::NUMBER:
        return std::is_same_v<T, int>;
      case ValueType::TEXT:
        return std::is_same_v<T, std::string>;
      default:
        return false;
    }
  }

  StorageType _value;
  ValueType _type;
};
}  // namespace model
