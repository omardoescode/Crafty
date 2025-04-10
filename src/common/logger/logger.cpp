#include "logger.h"
#include <unistd.h>
#include <utility>

namespace common {
Logger::Logger(std::string&& prefix, std::ostream& out, LogLevel lvl)
    : _prefix(std::move(prefix)), _level(lvl), _out(out) {
  std::cout << "Initializing Logger " << _prefix << std::endl;
}

Logger::Logger(const std::string& prefix, std::ostream& out, LogLevel lvl)
    : _prefix(prefix), _level(lvl), _out(out) {}

}  // namespace common
