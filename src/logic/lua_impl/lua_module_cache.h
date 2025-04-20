#pragma once
#include <filesystem>
#include <shared_mutex>
#include <string>
#include <unordered_map>

namespace logic::lua {
class LuaModuleCache {
public:
  LuaModuleCache(const LuaModuleCache&) = delete;
  LuaModuleCache& operator=(const LuaModuleCache&) = delete;

  /**
   * @brief Get singleton instance
   * @return returns the singleton instance
   */
  static LuaModuleCache& instance();

  /**
   * @brief Load a new module
   * @param category Category name
   * @param path Path to the module
   * @param reload Invaildate current cache and reload the module
   */
  void load_module(const std::string& category,
                   const std::filesystem::path& path, bool reload = false);

  /**
   * @brief Get a new module
   * @param category Category name
   * @return string containing all module content
   */
  const std::string& get_module(const std::string& category) const;

private:
  LuaModuleCache() = default;
  std::unordered_map<std::string, std::string> _modules;
  mutable std::shared_mutex _modules_mtx;
};
}  // namespace logic::lua
