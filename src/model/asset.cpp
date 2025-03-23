#include "asset.h"
#include "project.h"
#include "utils/serializable.h"

namespace model {
Asset::Asset(const IDManager::IDType& id, Project& project,
             const std::string& path)
    : Serializable(id, project), _path(path) {}
Asset::Asset(const IDManager::IDType& id, Project& project, std::string&& path)
    : Serializable(id, project), _path(std::move(path)) {}

const std::string& Asset::get_path() const { return _path; }
}  // namespace model
