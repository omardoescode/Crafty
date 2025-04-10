#pragma once

#include "id.h"

namespace model {
class Identifiable {
public:
  Identifiable(IDPtr id);
  ~Identifiable() = default;

  IDPtr id() const;

private:
  IDPtr _id;
};
}  // namespace model
