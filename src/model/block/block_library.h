#pragma once
#include <filesystem>
#include <memory>
#include <vector>
#include "block_storage.h"
#include "identity/id/id.h"

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
   * @brief a Config type for BlockLibrary Initialization
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

  std::vector<std::string> categories() const;
  std::vector<std::shared_ptr<const BlockDefinition>> category_blocks(
      const std::string& category) const;

  std::shared_ptr<const BlockDefinition> get_block_definition_by_id(
      IDPtr) const;

  std::shared_ptr<BlockInstance> create_dummy_instance();

private:
  std::unique_ptr<BlockStorage> _storage;
};  // namespace model
}  // namespace model
