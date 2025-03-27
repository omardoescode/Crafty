#pragma once
#include <filesystem>
std::filesystem::path construct_path(const char* path, const char* to_append);
std::filesystem::path construct_path(std::filesystem::path path,
                                     const char* to_append);

std::string generate_unique_filename(std::string extension);
