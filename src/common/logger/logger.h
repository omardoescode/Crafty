#pragma once

#include <iostream>
#include <string>
namespace common {
class Logger {
public:
  enum LogLevel { INFO = 0, WARN = 1, ERROR = 2 };

public:
  Logger(std::string, std::ostream& = std::cout, LogLevel = INFO);

  void set_loglevel(LogLevel);

  void info(std::string&& message);
  void warn(std::string&& message);
  void error(std::string&& message);

  void warn(LogLevel);

private:
  void print(std::string&& message, LogLevel lvl);

private:
  std::string _prefix;
  LogLevel _lvl;
  std::ostream& _out;
};
}  // namespace common
