#pragma once
#include <memory>
#include "character.h"
#include "script.h"
namespace logic {
class Interpreter {
public:
  /**
   * @brief An interpreter Status
   */
  enum Status { IDLE, RUNNING };

  /**
   * @brief default destructor
   */
  virtual ~Interpreter() = default;

  /**
   * @brief Register a script to be run in the next execution
   * @param script A weak pointer to the script
   *
   * @note The script will be removed after post execution
   */
  virtual void register_script(std::shared_ptr<model::Character> chr,
                               std::shared_ptr<model::Script> script) = 0;

  /**
   * @brief Execute The registered scripts so far
   *
   * @note All the scripts will be removed post execution
   */
  virtual void execute() = 0;

  /**
   * @brief getter for status
   */
  virtual Status status() = 0;
};
}  // namespace logic
