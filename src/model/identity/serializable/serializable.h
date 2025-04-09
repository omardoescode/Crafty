#pragma once

#include "identity/id/id.h"

namespace model {
class Project;
class Serializable {
public:
  Serializable(IDPtr id, Project& project);
  ~Serializable() = default;

  IDPtr id() const;

protected:
  Project& project_;

private:
  IDPtr _id;
};
}  // namespace model
