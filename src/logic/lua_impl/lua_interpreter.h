#pragma once
#include <memory>
#include <mutex>
#include <sol/sol.hpp>
#include <vector>
#include "character.h"
#include "events/event_dispatcher.h"
#include "interpreter.h"
#include "scope_table.h"
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
  LuaInterpreter(std::format_string<std::string&> module_path_fmt);

  /**
   * @brief Execute the registered scripts
   * @param chrs A list of all characters in the game
   */
  void execute(const std::vector<std::shared_ptr<model::Character>>&) override;

  /**
   * @brief Terminate the interpreter current execution
   */
  void terminate() override;

  /**
   * @brief getter for status
   */
  Status status() override;

private:
  std::atomic<Status> _status;

  std::shared_ptr<ScopeTable> _global_ctx;
  std::shared_ptr<ThreadPool> _thread_pool;
};
}  // namespace logic::lua
