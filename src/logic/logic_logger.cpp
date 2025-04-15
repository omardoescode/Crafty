#include "logic_logger.h"
#include "logger/logger.h"

common::Logger& logic_logger() {
  static common::Logger log("LOGIC");
  return log;
}
