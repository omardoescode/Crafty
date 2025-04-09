#pragma once
#include <memory>
#include <string>

namespace model {
/**
 * @brief Interface to all ID types
 */
class ID {
public:
  virtual ~ID() = default;

  // For usage with std::std and std::map?
  virtual bool operator==(const ID& rhs) const = 0;
  virtual bool operator<(const ID& rhs) const = 0;

  virtual std::string to_string() const = 0;
};

/**
 * @brief A type alias for a shared pointer for identifier
 */
typedef std::shared_ptr<ID> IDPtr;

/**
 * @brief Overload of the less than operator for IDPtr
 * @return true if `lhs` is less than `rhs`
 */
bool operator<(const IDPtr& lhs, const IDPtr& rhs);

/**
 * @brief Overload of the equality operator for IDPtr
 * @return true if `lhs` is equal to `rhs`
 */
bool operator==(const IDPtr& lhs, const IDPtr& rhs);
}  // namespace model
