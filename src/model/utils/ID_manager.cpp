#include "ID_manager.h"
#include <ctime>
#include <string>

namespace model {
IDManager::IDManager(const std::string& prefix, unsigned int start)
    : _prefix(prefix), _counter(start) {}
IDManager::IDManager(std::string&& prefix, unsigned int start)
    : _prefix(std::move(prefix)), _counter(start) {}

IDManager::IDType IDManager::generate_with_counter() {
  return _prefix + std::to_string(_counter.fetch_add(1));
}
}  // namespace model
