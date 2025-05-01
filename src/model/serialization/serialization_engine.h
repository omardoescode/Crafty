#pragma once

#include "serialization_context.h"
namespace model::serialization {
class SerializtionEngine {
public:
  virtual void serialize(SerializationContext&);
};
}  // namespace model::serialization
