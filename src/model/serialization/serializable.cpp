
#include "serializable.h"
namespace model {
Serializable::Serializable(IDPtr id, bool serialize)
    : Identifiable(id), to_serialize(serialize) {}

bool Serializable::is_to_serialize() const { return to_serialize; }
}  // namespace model
