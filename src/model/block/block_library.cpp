#include "block_library.h"
#include <filesystem>
#include <memory>
#include <stdexcept>
#include "model_logger.h"

namespace fs = std::filesystem;

namespace model {
BlockLibrary::BlockLibrary(std::unique_ptr<BlockStorage> definitions_repo,
                           const Config& config)
    : _storage(std::move(definitions_repo)) {
  model_logger().info("Block Library Initialization Started");

  if (!fs::exists(config.block_file_path))
    throw std::runtime_error(
        "BlockLibrary::BlockLibrary - config.block_file_path doesn't exist");
  _storage->load_definitions(config.block_file_path);

  model_logger().info("Block Library Initialized");
}

std::vector<std::string> BlockLibrary::categories() const {
  return _storage->categories();
}

std::vector<BlockDefPtr> BlockLibrary::category_blocks(
    const std::string& category) const {
  return _storage->get_definitions_by_category(category);
}
}  // namespace model
