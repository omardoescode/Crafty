#pragma once

#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
namespace common {
class Logger {
public:
  enum LogLevel { INFO = 0, WARN = 1, ERROR = 2 };

  Logger(std::string&&, std::ostream& = std::cout, LogLevel = INFO);
  Logger(const std::string&, std::ostream& = std::cout, LogLevel = INFO);

  template <typename... Args>
  void info(std::string_view fmt, Args&&... args) {
    log(INFO, fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  void warn(std::string_view fmt, Args&&... args) {
    log(WARN, fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  void error(std::string_view fmt, Args&&... args) {
    std::string log_message =
        generate_log_message(ERROR, fmt, std::forward<Args>(args)...);
    throw std::runtime_error(log_message);
  }

private:
  template <typename... Args>
  std::string generate_log_message(LogLevel level, std::string_view fmt,
                                   Args&&... args) {
    // TODO: Understand this part
    if constexpr (std::is_same_v<std::remove_cvref_t<decltype(fmt)>,
                                 const char*>) {
      // If format string is a string literal
      return std::format(fmt, std::forward<Args>(args)...);
    } else {
      // If format string is a std::string or std::string_view
      return std::vformat(fmt, std::make_format_args(args...));
    }
  }

  template <typename... Args>
  void log(LogLevel level, std::string_view fmt, Args&&... args) {
    auto log_message =
        generate_log_message(level, fmt, std::forward<Args>(args)...);

    if (level >= _level) std::print("[{}] {}\n", _prefix, log_message);
  }

private:
  std::string _prefix;
  LogLevel _level;
  std::ostream& _out;
};
}  // namespace common
