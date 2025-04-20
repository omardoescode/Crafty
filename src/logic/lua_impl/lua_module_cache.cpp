#include "lua_module_cache.h"
#include <filesystem>
#include <fstream>
#include <mutex>
#include <shared_mutex>
#include "logic_logger.h"
#include "model_logger.h"

namespace logic::lua {
LuaModuleCache& LuaModuleCache::instance() {
  static LuaModuleCache instance;
  return instance;
}

void LuaModuleCache::load_module(const std::string& category,
                                 const std::filesystem::path& path,
                                 bool reload) {
  {
    std::shared_lock lck(_modules_mtx);
    if (!reload && _modules.count(category) == 1) return;
  }

  if (!std::filesystem::exists(path))
    throw logic_logger().error(
        "Failed to initialize module {}: Invalid Path {}", category,
        path.string());

  std::ifstream file(path);
  {
    std::unique_lock lck(_modules_mtx);
    _modules[category] = std::string(std::istreambuf_iterator<char>(file),
                                     std::istreambuf_iterator<char>());
  }
}

const std::string& LuaModuleCache::get_module(
    const std::string& category) const {
  std::shared_lock lck(_modules_mtx);
  auto itr = _modules.find(category);
  if (itr == _modules.end())
    throw model_logger().error(
        "Module {} Not initialized yet",
        category);  // Man, I have been debugging a lot for his lol

  return itr->second;
}
}  // namespace logic::lua
