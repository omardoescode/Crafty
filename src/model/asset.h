#pragma once
#include <string>
#include "identity/id/id.h"
#include "identity/serializable/serializable.h"
namespace model {

class Project;
class Asset : public Serializable {
public:
  Asset(IDPtr id, Project& project, std::string&& path);

  const std::string& get_path() const;

private:
  std::string _path;
};

}  // namespace model
