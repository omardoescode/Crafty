#pragma once
#include <list>
#include <memory>
#include <mutex>
#include <set>
#include <sol/sol.hpp>
#include <thread>
#include "block/block_instance.h"
#include "block/input_slot_instance.h"
#include "events/event_dispatcher.h"
#include "interpreter.h"
#include "scope_table.h"
#include "script.h"

namespace logic {
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
  LuaInterpreter();
  ~LuaInterpreter();

  /**
   * @brief Register a script to be run in the next execution
   * @param script A weak pointer to the script
   */
  void register_script(std::shared_ptr<model::Script> script) override;

  /**
   * @brief
   */
  void execute() override;

  /**
   * @brief getter for status
   */

  Status status() override;

private:
  struct ThreadContext {
    std::thread thread;
    std::atomic<bool> is_running = std::atomic{true};
    std::weak_ptr<model::Script> script;
  };

  void initialize_state();
  void initialize_usertypes();
  void execute(std::shared_ptr<model::Script> script,
               std::shared_ptr<ScopeTable> current_table);
  model::Value execute_block(std::shared_ptr<model::BlockInstance> instance,
                             std::shared_ptr<ScopeTable> current_table);
  model::Value execute_input_slot(
      std::shared_ptr<model::InputSlotInstance> instance,
      std::shared_ptr<ScopeTable> current_table);

  sol::state _lua_state;
  std::mutex _lua_state_mtx;

  std::set<std::shared_ptr<model::Script>> _scripts;
  std::mutex _scripts_mtx;

  Status _status;
  std::mutex _status_mtx;

  std::list<std::unique_ptr<ThreadContext>> _threads;
  std::mutex _threads_mutex;

  std::shared_ptr<ScopeTable> _global_ctx;
  common::EventDispatcher::TokenP add_script_tkn;
  common::EventDispatcher::TokenP remove_script_tkn;
};
}  // namespace logic
