#include "serializable.h"
#include "identity/id/id.h"

namespace model {
Serializable::Serializable(IDPtr id, Project& project)
    : _id(id), project_(project) {}

IDPtr Serializable::id() const { return _id; }
}  // namespace model
