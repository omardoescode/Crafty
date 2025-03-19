#pragma once
#include <string>
namespace model {
class Asset {
public:
  Asset(const std::string& path);
  Asset(std::string&& path);

  const std::string get_path() const;

private:
  std::string _path;
};
}  // namespace model
