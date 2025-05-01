#include "lua_interpreter.h"
#include <atomic>
#include <format>
#include <memory>
#include <mutex>
#include <sol/sol.hpp>
#include "events/event_dispatcher.h"
#include "interpreter.h"
#include "logic_logger.h"
#include "lua_impl/lua_script_runtime.h"
#include "model_events.h"
#include "project_manager.h"
#include "scope_table.h"

static auto& dispatcher = common::EventDispatcher::instance();
static auto& mgr = model::ProjectManager::instance();

namespace logic::lua {
LuaInterpreter::LuaInterpreter(std::format_string<std::string&> module_path_fmt)
    : _status(IDLE), _global_ctx(std::make_shared<ScopeTable>()) {
  // Iniailzie module cache
  auto categories = mgr.block_lib()->categories();
  auto& module_cache = LuaModuleCache::instance();
  for (auto category : categories) {
    std::string file_path = std::format(module_path_fmt, category);
    std::filesystem::path path(file_path);
    module_cache.load_module(category, path);
    logic_logger().info("Lua Module {} initialized", file_path);
  }

  _thread_pool = std::make_shared<ThreadPool>(
      6);  // TODO: Find a better value for this number, I cannot take the
           // entire application threads though for there will be other threads
           // in other layers I guess
}

void LuaInterpreter::execute(
    const std::vector<std::shared_ptr<model::Character>>& chrs) {
  if (_status == RUNNING) return;

  // TODO: Fix this
  // _status.store(RUNNING);
  for (auto& chr : chrs) {
    for (auto& script : chr->scripts()) {
      _thread_pool->enqueue([&, this](std::shared_ptr<LuaStateManager> mgr) {
        auto new_table = std::make_shared<ScopeTable>(_global_ctx);
        LuaScriptRuntime runtime(chr, script, mgr, new_table);
        runtime.execute();
      });
    }
  }
}

Interpreter::Status LuaInterpreter::status() { return _status; }
void LuaInterpreter::terminate() {
  // TODO:
}
}  // namespace logic::lua
