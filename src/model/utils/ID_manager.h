#pragma once

#include <atomic>
#include <string>
namespace model {
class IDManager {
public:
  typedef std::string IDType;

public:
  IDManager(const std::string& prefix, unsigned int start = 0);
  IDManager(std::string&& prefix, unsigned int start = 0);

  /**
   * generates an auto-increment based ID
   */
  IDType generate_with_counter();

private:
  std::string _prefix;
  std::atomic<unsigned int> _counter;  // ensure thread safety
};
}  // namespace model
