#include "logger.h"
#include <unistd.h>
#include <utility>

static const char* reset_code = "\033[0m";
namespace common {

const char* Logger::reset_code = "\033[0m";
Logger::Logger(std::string&& prefix, std::ostream& out, LogLevel lvl)
    : _prefix(std::move(prefix)), _level(lvl), _out(out) {}

Logger::Logger(const std::string& prefix, std::ostream& out, LogLevel lvl)
    : _prefix(prefix), _level(lvl), _out(out) {}

}  // namespace common
