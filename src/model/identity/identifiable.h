#pragma once

#include "id.h"

namespace model {
class Identifiable {
public:
  Identifiable(IDPtr id);
  ~Identifiable() = default;

  IDPtr id() const;

  bool operator==(const Identifiable& rhs) const;
  bool operator<(const Identifiable& rhs) const;

private:
  IDPtr _id;
};
}  // namespace model
