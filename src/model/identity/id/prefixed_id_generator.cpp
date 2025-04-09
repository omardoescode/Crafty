
#include "prefixed_id_generator.h"
#include <ctime>
#include <memory>
#include <string>
#include "string_id.h"

namespace model {
PrefixedIDGenerator::PrefixedIDGenerator(const std::string& prefix,
                                         unsigned int start)
    : _prefix(prefix), _counter(start) {}
PrefixedIDGenerator::PrefixedIDGenerator(std::string&& prefix,
                                         unsigned int start)
    : _prefix(std::move(prefix)), _counter(start) {}

IDPtr PrefixedIDGenerator::generate_next() {
  return std::make_unique<StringID>(_prefix +
                                    std::to_string(_counter.fetch_add(1)));
}

}  // namespace model
