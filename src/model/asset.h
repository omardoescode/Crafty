#pragma once
#include <string>
#include "utils/ID_manager.h"
#include "utils/serializable.h"
namespace model {

class Project;
class Asset : public Serializable {
public:
  Asset(const IDManager::IDType&, Project&, const std::string&);
  Asset(const IDManager::IDType&, Project&, std::string&&);

  const std::string& get_path() const;

private:
  std::string _path;
};

}  // namespace model
