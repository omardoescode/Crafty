#include "type_mismatch.h"
#include <stdexcept>

namespace model::error {
TypeMismatchError::TypeMismatchError(const std::string& msg)
    : std::runtime_error(msg) {}
}  // namespace model::error
