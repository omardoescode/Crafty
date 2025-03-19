#include "json.h"
#include <fstream>
#include <stdexcept>

json parse_json(std::filesystem::path path) {
  std::fstream file(path);
  try {
    return json::parse(file);
  } catch (json::exception& exp) {
    throw std::runtime_error("Couldn't read the stream");
  }
}
