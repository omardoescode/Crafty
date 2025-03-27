#include "platform.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#if defined(_WIN32)
#include <windows.h>
std::filesystem::path get_executable_path() {
  char path[MAX_PATH];
  DWORD length = GetModuleFileNameA(nullptr, path, MAX_PATH);
  if (length == 0) throw std::runtime_error("Failed to get executable path");
  return path;
}

#elif defined(__linux__)
#include <limits.h>
#include <unistd.h>
std::filesystem::path get_executable_path() {
  char path[PATH_MAX];
  ssize_t length = readlink("/proc/self/exe", path, PATH_MAX);
  if (length == -1) throw std::runtime_error("Failed to get executable path");
  path[length] = '\0';
  return path;
}

#elif defined(__APPLE__)
#include <limits.h>
#include <mach-o/dyld.h>
std::filesystem::path get_executable_path() {
  char path[PATH_MAX];
  uint32_t size = PATH_MAX;
  if (_NSGetExecutablePath(path, &size) != 0) {
    throw std::runtime_error("Executable path buffer too small");
  }
  char resolved[PATH_MAX];
  realpath(path, resolved);
  return resolved;
}

#else
#error "Unsupported platform"
#endif
