#pragma once
#include "identity/identifiable.h"
namespace model {
class Serializable : public Identifiable {
public:
  Serializable(IDPtr id, bool serialize = false);

  /**
   * @brief Serilize the current entity
   *
   * TODO: Change the return type to something abstract, same as done with block
   * storage, figure out when building the serialization engine
   */
  // virtual void serialize() = 0;

  /**
   * @brief Check if this entity will be serialized or not
   * @return returns `true` is this entity is to be serialized
   */
  bool is_to_serialize() const;

private:
  bool to_serialize;
};
}  // namespace model
