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
 * @brief A type alias for a weak pointer for identifier
 */

typedef std::weak_ptr<ID> IDWPtr;
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

/**
 * @brief a Helper method to remove a matching shared pointer from a list of
 * weak pointers
 */
template <typename Container, typename ID>
void remove_weak_ptr(Container& container, ID target_id) {
  container.erase(std::remove_if(container.begin(), container.end(),
                                 [&](const auto& weak_ptr) {
                                   auto ptr = weak_ptr.lock();
                                   return ptr && ptr == target_id;
                                 }),
                  container.end());
}
}  // namespace model
