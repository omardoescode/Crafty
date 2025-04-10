#include "asset.h"
#include "project.h"

namespace model {
Asset::Asset(IDPtr id, std::string&& path, bool serialize)
    : Serializable(id, serialize), _path(std::move(path)) {}

const std::string& Asset::get_path() const { return _path; }
}  // namespace model
