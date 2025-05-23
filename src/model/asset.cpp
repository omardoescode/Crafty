#include "asset.h"

namespace model {
Asset::Asset(std::string&& path) : _path(std::move(path)) {}

const std::string& Asset::get_path() const { return _path; }
}  // namespace model
