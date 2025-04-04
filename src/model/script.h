#pragma once
#include <vector>
#include "utils/ID_manager.h"
#include "utils/serializable.h"

namespace model {
class Project;
class BlockInstance;

class Script : public Serializable {
public:
  Script(const IDManager::IDType& id, Project& project);

  void add_block_instance(const IDManager::IDType& id, int pos = -1);
  void remove_block_instance(const IDManager::IDType& id);
  bool has_block_instances() const;

  const std::vector<IDManager::IDType>& blocks() const;

private:
  std::vector<IDManager::IDType> _blocks;
};
}  // namespace model
