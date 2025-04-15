#pragma once

#include <unistd.h>
#include <cstdio>
#include <exception>
#include <iostream>
#include <mutex>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
namespace common {
class Logger {
public:
  enum LogLevel { INFO = 0, WARN = 1, ERROR = 2 };

  Logger(std::string&&, std::ostream& = std::cout, LogLevel = INFO);
  Logger(const std::string&, std::ostream& = std::cout, LogLevel = INFO);

  Logger(const Logger&) = delete;             // No copying
  Logger& operator=(const Logger&) = delete;  // No assignment

  template <typename... Args>
  void info(std::string_view fmt, Args&&... args) {
    log(INFO, fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  void warn(std::string_view fmt, Args&&... args) {
    log(WARN, fmt, std::forward<Args>(args)...);
  }
  template <typename ErrorType = std::runtime_error, typename... Args>
  void error(std::string_view fmt, Args&&... args) {
    static_assert(std::is_base_of<std::exception, ErrorType>());

    log(ERROR, fmt, std::forward<Args>(args)...);
    throw ErrorType("An error occurred, check logger output");
  }

private:
  template <typename... Args>
  void log(LogLevel level, std::string_view fmt, Args&&... args) {
    auto log_message = std::vformat(fmt, std::make_format_args(args...));
    if (level >= _level) {
      std::lock_guard lck(get_mutex_for_stream(_out));
      std::print(_out, "[{}] [{}] {}\n", _prefix, level_to_string(level),
                 log_message);
    }
  }

  static std::mutex& get_mutex_for_stream(std::ostream& stream);
  static std::string level_to_string(LogLevel level);

private:
  std::string _prefix;
  LogLevel _level;
  std::ostream& _out;
};
}  // namespace common
