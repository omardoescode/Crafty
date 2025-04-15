#include <stdexcept>
#include <string>

namespace logic::error {
class ValueNotFound : public std::runtime_error {
public:
  ValueNotFound(const std::string& label);
};
}  // namespace logic::error
