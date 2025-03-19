#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

json parse_json(std::filesystem::path stream);
