#pragma once
#include <filesystem>

/**
 * NOTE: Must be called in the main thread
 */
std::filesystem::path get_executable_path();
