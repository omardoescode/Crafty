#include "model_logger.h"
#include "logger/logger.h"

common::Logger& model_logger() {
  static common::Logger log("MODEL");
  return log;
}
