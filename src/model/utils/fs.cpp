#include "fs.h"
std::filesystem::path construct_path(const char* path, const char* to_append) {
  return std::filesystem::path(path) / to_append;
}

std::filesystem::path construct_path(std::filesystem::path path,
                                     const char* to_append) {
  return path / to_append;
}
