#pragma once
#include <string>
#include "serialization/serializable.h"
namespace model {

// TODO: Find a way to not paste all files to the destinations right away, but
// rather cache them and use a pool to do it
class Asset : public Serializable {
public:
  Asset(std::string&& path);

  const std::string& get_path() const;

private:
  std::string _path;
};
}  // namespace model
