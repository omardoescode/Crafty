#include "common_logger.h"
#include "logger/logger.h"

common::Logger& common_logger() {
  static common::Logger log("MODEL");
  return log;
}
