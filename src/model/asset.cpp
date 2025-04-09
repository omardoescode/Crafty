#include "asset.h"
#include "project.h"

namespace model {
Asset::Asset(IDPtr id, Project& project, std::string&& path)
    : Serializable(id, project), _path(std::move(path)) {}

const std::string& Asset::get_path() const { return _path; }
}  // namespace model
