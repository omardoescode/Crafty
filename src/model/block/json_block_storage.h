#include <map>
#include <memory>
#include "block/block_library.h"
#include "identity/id.h"
#include "identity/id_generator.h"
namespace model {
class JsonBlockStorage : public BlockStorage {
public:
  /**
   * @brief Constructor for JsonBlockStorage
   * @param id_generator A unique pointer to an ID generator used for block
   * definitions
   */
  JsonBlockStorage(IDGeneratorPtr id_generator);

  /**
   * @brief Load Definitions from a specified path to a JSON file
   * @param path The path has a json file that has
   */
  void load_definitions(const std::filesystem::path& path) override;

  /**
   * @brief Retrieve a block definition by the ID
   * @param id A unique pointer to an ID
   * @return A shared pointer to the block definition
   */
  BlockDefPtr get_definition_by_id(const IDPtr& id) const override;

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

private:
  std::map<IDPtr, BlockDefPtr> _defs;
  std::map<std::string, std::vector<BlockDefPtr>> _category_defs;
  IDGeneratorPtr _id_generator;
};
}  // namespace model
