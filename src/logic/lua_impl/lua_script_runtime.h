#pragma once
#include <memory>
#include "block/block_instance.h"
#include "block/input_slot_instance.h"
#include "block/value.h"
#include "lua_impl/lua_state_manager.h"
#include "scope_table.h"
#include "script.h"
namespace logic::lua {
/**
 * @brief A script execution environment
 */
class LuaScriptRuntime {
public:
  /**
   * @brief Constructor
   * @param script A shared pointer to the script to execute
   * @param scope The scope to put variables on.
   * @note The scope must be new one to avoid inconsistencies
   */
  LuaScriptRuntime(std::shared_ptr<model::Script> script,
                   std::shared_ptr<LuaStateManager> state_mgr,
                   std::shared_ptr<ScopeTable> scope);

  /**
   * @brief Execute the script
   */
  void execute();

  /**
   * @brief Execute the specified block instance
   * @param block a shared pointer to the block instanc
   * @return The value the block execution returns
   */
  model::Value execute_block(std::shared_ptr<model::BlockInstance> block);

  /**
   * @brief Execute the specified input slot instance
   * @param input a shared pointer to the input slot instance
   * @return The value the input slot returns
   */
  model::Value execute_input_slot(
      std::shared_ptr<model::InputSlotInstance> input);

private:
  sol::table prepare_context_table();

  std::shared_ptr<LuaStateManager> _state_mgr;
  std::shared_ptr<model::Character> _character;
  std::shared_ptr<ScopeTable> _scope;
  std::shared_ptr<model::Script> _script;
};
}  // namespace logic::lua
