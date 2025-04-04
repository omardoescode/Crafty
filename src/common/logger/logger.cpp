#include "logger.h"
#include <unistd.h>
#include <utility>

static const char* reset_code = "\033[0m";
namespace common {

const char* Logger::color_codes[] = {
    "\033[32m",  // INFO -> Green
    "\033[33m",  // WARN -> Yellow
    "\033[31m"   // ERROR -> Red
};

const char* Logger::reset_code = "\033[0m";
Logger::Logger(std::string&& prefix, std::ostream& out, LogLevel lvl)
    : _prefix(std::move(prefix)), _lvl(lvl), _out(out) {}

Logger::Logger(const std::string& prefix, std::ostream& out, LogLevel lvl)
    : _prefix(prefix), _lvl(lvl), _out(out) {}
void Logger::set_loglevel(Logger::LogLevel lvl) { _lvl = lvl; }

bool Logger::is_terminal() const {
  bool is_terminal = isatty(fileno(stdout));  // Works for Linux/macOS
#ifdef _WIN32
  is_terminal = _isatty(_fileno(stdout));  // Windows compatibility
#endif
  return is_terminal;
}
}  // namespace common
