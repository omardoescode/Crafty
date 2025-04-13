#include "logger.h"
#include <map>
#include <memory>
#include <utility>

namespace common {
Logger::Logger(std::string&& prefix, std::ostream& out, LogLevel lvl)
    : _prefix(std::move(prefix)), _level(lvl), _out(out) {
  std::lock_guard lck(get_mutex_for_stream(_out));
  std::print(_out, "{} Logger Initialized\n", _prefix);
}

Logger::Logger(const std::string& prefix, std::ostream& out, LogLevel lvl)
    : _prefix(prefix), _level(lvl), _out(out) {
  std::lock_guard lck(get_mutex_for_stream(_out));
  std::print(_out, "{} Logger Initialized\n", _prefix);
}

std::mutex& Logger::get_mutex_for_stream(std::ostream& stream) {
  static std::mutex map_mutex;
  static std::map<std::ostream*, std::unique_ptr<std::mutex>> stream_mutexes;

  std::lock_guard<std::mutex> lock(map_mutex);
  auto& mutex_ptr = stream_mutexes[&stream];
  if (!mutex_ptr) {
    mutex_ptr = std::make_unique<std::mutex>();
  }
  return *mutex_ptr;
}
std::string Logger::level_to_string(Logger::LogLevel level) {
  static constexpr std::string_view levels[] = {"INFO", "WARN", "ERROR"};
  return std::string(levels[level]);
}

}  // namespace common
