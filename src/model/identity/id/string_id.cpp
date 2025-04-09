#include "string_id.h"

namespace model {
StringID::StringID(const std::string& value) : _value(value) {}
StringID::StringID(std::string&& value) : _value(std::move(value)) {}

bool StringID::operator==(const ID& rhs) const {
  if (const auto* other = dynamic_cast<const StringID*>(&rhs)) {
    return _value == other->_value;
  }
  return false;
}
bool StringID::operator<(const ID& rhs) const {
  if (const auto* other = dynamic_cast<const StringID*>(&rhs)) {
    return _value < other->_value;
  }
  return typeid(*this).name() < typeid(rhs).name();
}
std::string StringID::to_string() const { return _value; }
}  // namespace model
