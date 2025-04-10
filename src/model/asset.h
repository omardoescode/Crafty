#pragma once
#include <string>
#include "serialization/serializable.h"
namespace model {

class Project;
class Asset : public Serializable {
public:
  Asset(IDPtr id, std::string&& path, bool serialize = true);

  const std::string& get_path() const;

private:
  std::string _path;
};

}  // namespace model
