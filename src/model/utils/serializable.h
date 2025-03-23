#pragma once

#include "utils/ID_manager.h"

namespace model {
class Project;
class Serializable {
public:
  Serializable(IDManager::IDType id, Project& project);

  const IDManager::IDType& id() const;

protected:
  Project& project_;

private:
  const IDManager::IDType _id;
};
}  // namespace model
