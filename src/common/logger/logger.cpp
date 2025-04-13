#include "logger.h"
#include <unistd.h>
#include <utility>

namespace common {
Logger::Logger(std::string&& prefix, std::ostream& out, LogLevel lvl)
    : _prefix(std::move(prefix)), _level(lvl), _out(out) {
  std::print("{} Logger Initialized", _prefix);
}

Logger::Logger(const std::string& prefix, std::ostream& out, LogLevel lvl)
    : _prefix(prefix), _level(lvl), _out(out) {
  std::print("{} Logger Initialized", _prefix);
}

}  // namespace common
