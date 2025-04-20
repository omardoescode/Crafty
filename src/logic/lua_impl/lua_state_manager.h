#pragma once
#include <sol/sol.hpp>
#include "lua_impl/lua_module_cache.h"
namespace logic::lua {
class LuaStateManager {
public:
  /**
   * @brief Get the state
   * @return a lua state
   */
  sol::state& state();

  /**
   * @brief Iniailialize a state
   * @description A state initialization includes
   * 1. Loading Core Modules (ex: Maths lib)
   * 2. Loading Modules Information (via module_cache)
   * 2. Initialize Usertypes
   */
  void initialize(LuaModuleCache& cache);

private:
  sol::state _state;
};
}  // namespace logic::lua
