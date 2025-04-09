#include <atomic>
#include "id_generator.h"

namespace model {

class PrefixedIDGenerator : public IDGenerator {
public:
  PrefixedIDGenerator(const std::string& prefix, unsigned int start = 0);
  PrefixedIDGenerator(std::string&& prefix, unsigned int start = 0);

  /**
   * generates an auto-increment based ID
   */
  IDPtr generate_next() override;

private:
  std::string _prefix;
  std::atomic<unsigned int> _counter;  // ensure thread safety
};
}  // namespace model
