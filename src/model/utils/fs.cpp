#include "fs.h"
std::filesystem::path construct_path(const char* path, const char* to_append) {
  return std::filesystem::path(path) / to_append;
}

std::filesystem::path construct_path(std::filesystem::path path,
                                     const char* to_append) {
  return path / to_append;
}

std::string get_current_timestamp() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_c), "%Y%m%d%H%M%S");
  return ss.str();
}

std::string generate_unique_filename(std::string extension) {
  return get_current_timestamp() + extension;
}
