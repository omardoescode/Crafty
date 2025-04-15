#include "identifiable.h"

namespace model {
Identifiable::Identifiable(IDPtr id) : _id(id) {}

IDPtr Identifiable::id() const { return _id; }

bool Identifiable::operator==(const Identifiable& rhs) const {
  return _id == rhs._id;
}
bool Identifiable::operator<(const Identifiable& rhs) const {
  return _id == rhs._id;
}
}  // namespace model
