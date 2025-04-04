#include "logger.h"
#include <unistd.h>
#include <utility>

static const char* color_codes[] = {
    "\033[32m",  // INFO -> Green
    "\033[33m",  // WARN -> Yellow
    "\033[31m"   // ERROR -> Red
};
static const char* reset_code = "\033[0m";
namespace common {
Logger::Logger(std::string prefix, std::ostream& out, LogLevel lvl)
    : _prefix(prefix), _lvl(lvl), _out(out) {}

void Logger::set_loglevel(Logger::LogLevel lvl) { _lvl = lvl; }

void Logger::operator()(std::string&& message) {
  print(std::move(message), _lvl);
}
void Logger::operator()(const std::string& message) { print(message, _lvl); }

void Logger::warn(std::string&& message) { print(std::move(message), WARN); }
void Logger::info(std::string&& message) { print(std::move(message), INFO); }
void Logger::error(std::string&& message) { print(std::move(message), ERROR); }

void Logger::warn(const std::string& message) { print(message, WARN); }
void Logger::info(const std::string& message) { print(message, INFO); }
void Logger::error(const std::string& message) { print(message, ERROR); }

void Logger::print(std::string&& message, LogLevel lvl) {
  if (lvl < _lvl) return;
  bool is_terminal = isatty(fileno(stdout));  // Works for Linux/macOS
#ifdef _WIN32
  is_terminal = _isatty(_fileno(stdout));  // Windows compatibility
#endif

  if (is_terminal) {
    _out << color_codes[lvl] << "[" << _prefix << "] " << std::move(message)
         << reset_code << std::endl;
  } else {
    _out << "[" << _prefix << "] " << std::move(message) << std::endl;
  }
}

void Logger::print(const std::string& message, LogLevel lvl) {
  if (lvl < _lvl) return;
  bool is_terminal = isatty(fileno(stdout));  // Works for Linux/macOS
#ifdef _WIN32
  is_terminal = _isatty(_fileno(stdout));  // Windows compatibility
#endif

  if (is_terminal) {
    _out << color_codes[lvl] << "[" << _prefix << "] " << message << reset_code
         << std::endl;
  } else {
    _out << "[" << _prefix << "] " << message << std::endl;
  }
}
}  // namespace common
