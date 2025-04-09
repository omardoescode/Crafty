#include "block_library.h"
#include <filesystem>
#include <memory>
#include <stdexcept>

namespace fs = std::filesystem;

namespace model {
BlockLibrary::BlockLibrary(std::unique_ptr<BlockStorage> definitions_repo,
                           const Config& config)
    : _storage(std::move(definitions_repo)) {
  if (!fs::exists(config.block_file_path))
    throw std::runtime_error(
        "BlockLibrary::BlockLibrary - config.block_file_path doesn't exist");
  _storage->load_definitions(config.block_file_path);
}

}  // namespace model
