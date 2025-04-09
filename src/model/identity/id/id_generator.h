#pragma once
#include <memory>
#include "id.h"

namespace model {
class IDGenerator {
public:
  ~IDGenerator() = default;

  /**
   * @brief Generate a new unique identifer
   * @return a shared pointer to the id generated
   */
  virtual IDPtr generate_next() = 0;
};

/**
 * @brief A type alias for a unique pointer to an ID Generator
 */
typedef std::unique_ptr<IDGenerator> IDGeneratorPtr;
}  // namespace model
