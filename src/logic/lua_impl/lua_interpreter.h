#pragma once
#include <memory>
#include <mutex>
#include <set>
#include <sol/sol.hpp>
#include "block/block_instance.h"
#include "block/input_slot_instance.h"
#include "character.h"
#include "events/event_dispatcher.h"
#include "interpreter.h"
#include "lua_state_manager.h"
#include "scope_table.h"
#include "script.h"
#include "thread_pool.h"

namespace logic::lua {
/**
 * @brief A interpreter that uses LUA language
 *
 * @description How does the interpreter work?
 * A user registers a script right before execution. A script gets removed once
 * executed
 *
 * Upon execution, a thread (call runtime thread) is created to handle the
 * entire execution thing
 *
 * This runtime thread will initialize other child threads, each corresponding
 * to a script and will overwrite interpreter state
 */
class LuaInterpreter : public Interpreter {
public:
  /**
   * @brief Default Constructor
   * @param module_path_fmt A format string used to get the path of the
   * categories, with arguments
   *  1. category name
   * @param pool_size The maximum number of possible threads going together
   */
  LuaInterpreter(std::string module_path_fmt);

  /**
   * @brief Register a script to be run in the next execution
   * @param script A weak pointer to the script
   */
  void register_script(std::shared_ptr<model::Script> script) override;

  /**
   * @brief Execute the registered scripts
   */
  void execute() override;

  /**
   * @brief getter for status
   */

  Status status() override;

private:
  model::Value execute_block(std::shared_ptr<LuaStateManager> state_mgr,
                             std::shared_ptr<model::Character> character,
                             std::shared_ptr<model::BlockInstance> instance,
                             std::shared_ptr<ScopeTable> current_table);
  void execute(std::shared_ptr<LuaStateManager> state_mgr,
               std::shared_ptr<model::Script> script,
               std::shared_ptr<ScopeTable> parent_table);
  model::Value execute_input_slot(
      std::shared_ptr<LuaStateManager> mgr,
      std::shared_ptr<model::Character> character,
      std::shared_ptr<model::InputSlotInstance> instance,
      std::shared_ptr<ScopeTable> current_table);

  std::set<std::shared_ptr<model::Script>> _scripts;
  std::mutex _scripts_mtx;

  Status _status;
  std::mutex _status_mtx;

  std::shared_ptr<ScopeTable> _global_ctx;
  common::EventDispatcher::TokenP add_script_tkn;
  common::EventDispatcher::TokenP remove_script_tkn;

  std::shared_ptr<ThreadPool> _thread_pool;
};
}  // namespace logic::lua
