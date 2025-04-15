#include <stdexcept>
#include <string>

namespace model::error {
class TypeMismatchError : public std::runtime_error {
public:
  TypeMismatchError(const std::string& msg);
};
}  // namespace model::error
