#pragma once
#include <string>
#include <vector>
#include "serialization/serializable.h"
namespace model::serialization {
class SerializationContext {
public:
  virtual void operator<<(const std::string& string);
  virtual void operator<<(int number);
  virtual void operator<<(double number);
  virtual void operator<<(bool boolean);
  virtual void operator<<(std::vector<Serializable>);
  virtual void set_field_name(const std::string& field_name);
};
}  // namespace model::serialization
