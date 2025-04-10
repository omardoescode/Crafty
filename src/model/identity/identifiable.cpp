#include "identifiable.h"

namespace model {
Identifiable::Identifiable(IDPtr id) : _id(id) {}

IDPtr Identifiable::id() const { return _id; }
}  // namespace model
