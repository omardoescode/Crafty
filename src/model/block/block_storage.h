#include <filesystem>
#include <memory>
#include <vector>
#include "block_definition.h"
#include "identity/id.h"

namespace model {
class BlockStorage {
public:
  virtual ~BlockStorage() = default;

  /**
   * @brief Load Definitions from a specified path
   * @param path The path containing block definitions
   */
  virtual void load_definitions(const std::filesystem::path& path) = 0;

  /**
   * @brief Retrieve a block definition by the ID
   * @param id A unique pointer to an ID
   * @return A shared pointer to the block definition
   */
  virtual BlockDefPtr get_definition_by_id(const IDPtr& id) const = 0;

  /**
   * @brief Retrieve all the block definitions in the specified category
   * @param category The category to filter by
   * @return A vector of shared pointers to all the block definitions in the
   * specified category
   */
  virtual std::vector<std::shared_ptr<const BlockDefinition>>
  get_definitions_by_category(const std::string& category) const = 0;

  /**
   * @brief Retrieve all categories names
   * @return A vector of the categories names
   */
  virtual std::vector<std::string> categories() const = 0;
};

}  // namespace model
