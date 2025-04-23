#pragma once
#include <filesystem>
#include <memory>
#include <vector>
#include "block_storage.h"

/**
 * @brief a Facade for Block Storage, an interface for handling blocks
 */
namespace model {
class BlockDefinition;
class BlockInstance;
class Project;

class BlockLibrary {
public:
  /**
   * @brief a Config struct for BlockLibrary Initialization
   */
  struct Config {
    std::filesystem::path block_file_path;
  };

  /**
   * @brief Constructor for BlockLibrary
   * @param storage A repository for the block definitions
   * @param config A `BlockLibrary::Config` struct for initialization
   */
  BlockLibrary(std::unique_ptr<BlockStorage> storage, const Config& config);

  /**
   * @brief Retrieve categories
   * @return A vector of categories names
   */
  std::vector<std::string> categories() const;

  /**
   * @brief Get a specified category blocks
   * @param category The name of the specified category
   * @return  A vector of the blocks of this category
   */
  std::vector<BlockDefPtr> category_blocks(const std::string& category) const;

private:
  std::unique_ptr<BlockStorage> _storage;
};
}  // namespace model
