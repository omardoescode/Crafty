#include "id.h"

namespace model {
class StringID : public ID {
public:
  StringID(const std::string&);
  StringID(std::string&&);

  bool operator==(const ID& rhs) const override;
  bool operator<(const ID& rhs) const override;
  std::string to_string() const override;

private:
  std::string _value;
};
}  // namespace model
