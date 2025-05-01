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
   * @brief Execute The registered scripts so far
   */
  virtual void execute(
      const std::vector<std::shared_ptr<model::Character>>& chrs) = 0;

  /**
   * @brief Terminate the interpreter current execution
   */
  virtual void terminate() = 0;

  /**
   * @brief getter for status
   */
  virtual Status status() = 0;
};
}  // namespace logic
