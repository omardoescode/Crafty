#pragma once

#include <filesystem>
std::filesystem::path upload_file(const char* filter_list_title,
                                  const char* filter_list,
                                  const char* default_path);
