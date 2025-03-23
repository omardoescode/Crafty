#include "serializable.h"

namespace model {
Serializable::Serializable(IDManager::IDType id, Project& project)
    : _id(id), project_(project) {}

const IDManager::IDType& Serializable::id() const { return _id; }
}  // namespace model
