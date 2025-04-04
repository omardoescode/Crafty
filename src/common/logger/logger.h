#pragma once

#include <unistd.h>
#include <iostream>
#include <ostream>
#include <string>
namespace common {
class Logger {  // TODO: Change the design to cout anything, not only strings
public:
  enum LogLevel { INFO = 0, WARN = 1, ERROR = 2 };

public:
  Logger(std::string&&, std::ostream& = std::cout, LogLevel = INFO);
  Logger(const std::string&, std::ostream& = std::cout, LogLevel = INFO);

  void set_loglevel(LogLevel);

  template <typename T>
  void info(T&& message) {
    print(std::move(message), INFO);
  }
  template <typename T>
  void warn(T&& message) {
    print(std::move(message), WARN);
  }
  template <typename T>
  void error(T&& message) {
    print(std::move(message), ERROR);
  }

  template <typename T>
  void info(const T& message) {
    print(message, INFO);
  }
  template <typename T>
  void warn(const T& message) {
    print(message, WARN);
  }
  template <typename T>
  void error(const T& message) {
    print(message, ERROR);
  }

  template <typename T>
  void operator()(T&& message) {
    print(std::move(message), _lvl);
  }
  template <typename T>
  void operator()(const T& message) {
    print(message, _lvl);
  }

private:
  template <typename T>
  void print(T&& message, LogLevel lvl) {
    if (lvl < _lvl) return;
    if (is_terminal())
      _out << color_codes[lvl] << "[" << _prefix << "] " << std::move(message)
           << reset_code << std::endl;
    else
      _out << "[" << _prefix << "] " << std::move(message) << std::endl;
    std::flush(_out);
  }
  template <typename T>
  void print(const T& message, LogLevel lvl) {
    if (lvl < _lvl) return;
    if (is_terminal())
      _out << color_codes[lvl] << "[" << _prefix << "] " << message
           << reset_code << std::endl;
    else
      _out << "[" << _prefix << "] " << std::move(message) << std::endl;
  }

  bool is_terminal() const;

private:
  std::string _prefix;
  LogLevel _lvl;
  std::ostream& _out;

  static const char* color_codes[];
  static const char* reset_code;
};
}  // namespace common
