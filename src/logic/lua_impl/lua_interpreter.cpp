#include "lua_interpreter.h"
#include <format>
#include <memory>
#include <mutex>
#include <print>
#include <sol/sol.hpp>
#include <stdexcept>
#include "block/input_slot_instance.h"
#include "block/value.h"
#include "character.h"
#include "events/event_dispatcher.h"
#include "logic_logger.h"
#include "lua_impl/lua_module_cache.h"
#include "lua_impl/lua_state_manager.h"
#include "model_events.h"
#include "project_manager.h"
#include "scope_table.h"

static auto& dispatcher = common::EventDispatcher::instance();
static auto& mgr = model::ProjectManager::instance();

namespace logic::lua {
LuaInterpreter::LuaInterpreter(std::string module_path_fmt)
    : _status(IDLE), _global_ctx(std::make_shared<ScopeTable>()) {
  remove_script_tkn = dispatcher.subscribe<model::events::beforeScriptDeleted>(
      [this](auto evt) { _scripts.erase(evt->script); });

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

void LuaInterpreter::register_script(std::shared_ptr<model::Script> script) {
  std::lock_guard lck(_scripts_mtx);
  _scripts.insert(script);
}

void LuaInterpreter::execute() {
  for (auto& script : _scripts) {
    _thread_pool->enqueue([&, this](std::shared_ptr<LuaStateManager> mgr) {
      execute(mgr, script, _global_ctx);
    });
  }

  // clear all scripts reference, a script still has a
  // valid shared reference anyway
  _scripts.clear();
}

void LuaInterpreter::execute(std::shared_ptr<LuaStateManager> state_mgr,
                             std::shared_ptr<model::Script> script,
                             std::shared_ptr<ScopeTable> parent_table) {
  auto scope = std::make_shared<ScopeTable>(parent_table);
  static auto& instance_store = mgr.project()->instances_store();
  auto instances = script->blocks();
  auto character = script->character();
  for (auto instance_w : instances) {
    auto instance_id = instance_w.lock();
    if (!instance_id) continue;

    auto instance = instance_store.get_entity(instance_id);
    execute_block(state_mgr, character, instance, scope);
  }
}

model::Value LuaInterpreter::execute_block(
    std::shared_ptr<LuaStateManager> state_mgr,
    std::shared_ptr<model::Character> character,
    std::shared_ptr<model::BlockInstance> instance,
    std::shared_ptr<ScopeTable> current_table) {
  // Handle inputs
  for (std::shared_ptr<model::InputSlotInstance> input : instance->inputs()) {
    // TODO: TEST THIS PART
    model::Value value =
        execute_input_slot(state_mgr, character, input, current_table);
    current_table->add_variable(input->def().label(), value);
  }

  {
    // Create a new Lua environment for this execution
    auto& state = state_mgr->state();

    // Create the argument list
    sol::table block_ctx = state.create_table();
    block_ctx["scope"] = current_table.get();
    block_ctx["character"] = character.get();
    // Execute the script in this environment
    try {
      sol::protected_function func =
          state[instance->def()->category()][instance->def()->data_id()];
      auto result = func(block_ctx);

      if (!result.valid()) {
        sol::error err = result;
        throw logic_logger().error("Lua execution error: {}", err.what());
      }

      return result;
    } catch (const sol::error& e) {
      throw logic_logger().error("Lua execution error: {}", e.what());
    }
  }

  throw std::runtime_error("It shouldn't make it here?");
}

Interpreter::Status LuaInterpreter::status() { return _status; }

model::Value LuaInterpreter::execute_input_slot(
    std::shared_ptr<LuaStateManager> state_mgr,
    std::shared_ptr<model::Character> character,
    std::shared_ptr<model::InputSlotInstance> instance,
    std::shared_ptr<ScopeTable> current_table) {
  if (instance->has_block()) {
    auto block_id = instance->block_id();
    auto block = mgr.project()->instances_store().get_entity(block_id);
    return execute_block(state_mgr, character, block, current_table);
  }
  return instance->value();
}
}  // namespace logic::lua
