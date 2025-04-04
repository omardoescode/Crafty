#pragma once

#include <iostream>
#include <string>
namespace common {
class Logger {  // TODO: Change the design to cout anything, not only strings
public:
  enum LogLevel { INFO = 0, WARN = 1, ERROR = 2 };

public:
  Logger(std::string, std::ostream& = std::cout, LogLevel = INFO);

  void set_loglevel(LogLevel);

  void info(std::string&& message);
  void warn(std::string&& message);
  void error(std::string&& message);

  void info(const std::string& message);
  void warn(const std::string& message);
  void error(const std::string& message);

  void operator()(std::string&& message);
  void operator()(const std::string& message);

private:
  void print(std::string&& message, LogLevel lvl);
  void print(const std::string& message, LogLevel lvl);

private:
  std::string _prefix;
  LogLevel _lvl;
  std::ostream& _out;
};
}  // namespace common
