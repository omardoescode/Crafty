#include "ui_logger.h"
#include "logger/logger.h"

common::Logger ui_logger() {
  static common::Logger log("UI");
  return log;
}
