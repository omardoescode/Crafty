#include <map>
#include "block/block_library.h"
namespace model {
class JsonBlockStorage : public BlockStorage {
public:
  /**
   * @brief Constructor for JsonBlockStorage
   * @param id_generator A unique pointer to an ID generator used for block
   * definitions
   */
  JsonBlockStorage();

  /**
   * @brief Load Definitions from a specified path to a JSON file
   * @param path The path has a json file that has
   */
  void load_definitions(const std::filesystem::path& path) override;

  /**
   * @brief Retrieve all the block definitions in the specified category
   * @param category The category to filter by
   * @return A vector of shared pointers to all the block definitions in the
   * specified category
   */
  std::vector<BlockDefPtr> get_definitions_by_category(
      const std::string& category) const override;

  /**
   * @brief Retrieve all categories names
   * @return A vector of the categories names
   */
  std::vector<std::string> categories() const override;

  /**
   * @brief Retrieve a block definition by the ID
   * @param id A definition id
   * @return A shared pointer to the block definition
   */
  BlockDefPtr get_definition_by_id(std::string id) const override;

private:
  std::map<std::string, BlockDefPtr> _defs;
  std::map<std::string, std::vector<BlockDefPtr>> _category_defs;
};
}  // namespace model
