#include "value_not_found.h"
#include <stdexcept>

namespace logic::error {
ValueNotFound::ValueNotFound(const std::string& label)
    : std::runtime_error("Value not found: " + label) {}
}  // namespace logic::error
