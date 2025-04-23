#include "lua_interpreter.h"
#include <algorithm>
#include <format>
#include <memory>
#include <mutex>
#include <sol/sol.hpp>
#include "events/event_dispatcher.h"
#include "logic_logger.h"
#include "lua_impl/lua_script_runtime.h"
#include "model_events.h"
#include "project_manager.h"
#include "scope_table.h"

static auto& dispatcher = common::EventDispatcher::instance();
static auto& mgr = model::ProjectManager::instance();

namespace logic::lua {
LuaInterpreter::LuaInterpreter(std::string module_path_fmt)
    : _status(IDLE), _global_ctx(std::make_shared<ScopeTable>()) {
  remove_script_tkn = dispatcher.subscribe<model::events::beforeScriptDeleted>(
      [this](auto evt) {
        _scripts_ctx.erase(
            std::remove_if(
                _scripts_ctx.begin(), _scripts_ctx.end(),
                [&](auto& pair) { return pair.second == evt->script; }),
            _scripts_ctx.end());
      });

  // Iniailzie module cache
  auto categories = mgr.block_lib()->categories();
  auto& module_cache = LuaModuleCache::instance();
  for (auto& category : categories) {
    std::string file_path =
        std::vformat(module_path_fmt, std::make_format_args(category));
    std::filesystem::path path(file_path);
    module_cache.load_module(category, path);
    logic_logger().info("Lua Module {} initialized", file_path);
  }

  _thread_pool = std::make_shared<ThreadPool>();
}

void LuaInterpreter::register_script(std::shared_ptr<model::Character> chr,
                                     std::shared_ptr<model::Script> script) {
  std::lock_guard lck(_scripts_mtx);
  _scripts_ctx.push_back({chr, script});
}

void LuaInterpreter::execute() {
  for (auto& [chr, script] : _scripts_ctx) {
    _thread_pool->enqueue([&, this](std::shared_ptr<LuaStateManager> mgr) {
      auto new_table = std::make_shared<ScopeTable>(_global_ctx);
      LuaScriptRuntime runtime(chr, script, mgr, new_table);
      runtime.execute();
    });
  }

  // clear all scripts reference, a script still has a
  // valid shared reference anyway
  _scripts_ctx.clear();
}

Interpreter::Status LuaInterpreter::status() { return _status; }
}  // namespace logic::lua
