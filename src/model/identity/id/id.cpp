#include "identity/id/id.h"
namespace model {
bool operator<(const IDPtr& lhs, const IDPtr& rhs) {
  if (lhs && rhs) return *lhs < *rhs;
  if (lhs) return true;
  if (rhs) return false;
  return false;
}

bool operator==(const IDPtr& lhs, const IDPtr& rhs) {
  if (lhs && rhs) return *lhs == *rhs;
  return false;
}
}  // namespace model
