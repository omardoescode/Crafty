#include "lua_script_runtime.h"
#include <sol/forward.hpp>
#include "block/input_slot_instance.h"
#include "character.h"
#include "logic_logger.h"
#include "lua_impl/lua_state_manager.h"
#include "project_manager.h"

static auto& mgr = model::ProjectManager::instance();
namespace logic::lua {
LuaScriptRuntime::LuaScriptRuntime(std::shared_ptr<model::Script> script,
                                   std::shared_ptr<LuaStateManager> state_mgr,
                                   std::shared_ptr<ScopeTable> scope)
    : _state_mgr(state_mgr), _scope(scope), _script(script) {
  _character = script->character();
}

void LuaScriptRuntime::execute() {
  assert(_script && "Invalid Script");
  auto& instance_store = mgr.project()->instances_store();

  auto instances = _script->blocks();
  auto character = _script->character();

  // Execute The scripts
  for (auto instance_w : instances) {
    auto instance_id = instance_w.lock();
    assert(instance_id && "Invalid Instance ID");

    auto instance = instance_store.get_entity(instance_id);
    execute_block(instance);
  }
}

model::Value LuaScriptRuntime::execute_block(
    std::shared_ptr<model::BlockInstance> instance) {
  for (auto input : instance->inputs()) {
    model::Value value = execute_input_slot(input);
    _scope->add_variable(input->def().label(), value);
  }

  auto& state = _state_mgr->state();
  auto context = prepare_context_table();

  sol::protected_function func =
      state[instance->def()->category()][instance->def()->data_id()];
  auto result = func(context);

  if (!result.valid()) {
    sol::error err = result;
    throw logic_logger().error("Lua execution error: {}", err.what());
  }
  return result;
}

model::Value LuaScriptRuntime::execute_input_slot(
    std::shared_ptr<model::InputSlotInstance> instance) {
  auto& instance_store = mgr.project()->instances_store();
  if (instance->has_block()) {
    auto block_id = instance->block_id();
    auto block = instance_store.get_entity(block_id);
    return execute_block(block);
  }
  return instance->value();
}

sol::table LuaScriptRuntime::prepare_context_table() {
  auto& state = _state_mgr->state();
  sol::table block_ctx = state.create_table();
  block_ctx["scope"] = _scope.get();
  block_ctx["character"] = _character.get();
  return block_ctx;
}
}  // namespace logic::lua
