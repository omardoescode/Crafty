#pragma once
#include "block/block_definition.h"
#include "utils/json.h"
namespace model {
class BlockLibrary {
public:
  static const char* block_folder_pathname;
  static const char* block_initfile_pathname;

public:
  // remove copying for singltons
  BlockLibrary(BlockLibrary&) = delete;
  BlockLibrary& operator=(BlockLibrary&) = delete;

  // Get instance
  static BlockLibrary& instance();

  // Initialize the block library
  void initialize();

  void reload();

  const std::vector<std::string>& categories() const;
  std::vector<std::shared_ptr<const BlockDefinition>> category_blocks(
      const std::string& category) const;

  std::shared_ptr<const BlockDefinition> get_block_definition_by_id(
      const BlockDefinitionID&) const;

private:
  BlockLibrary();

  void _load_blocks();
  void _load_blocks_initfile();
  void _load_blocks_json_data();
  BlockDefinition _parse_block(const json&, const std::string&);

private:
  bool _initialized;
  json _initfile;
  // TODO: Optimize category block queries by changing this design layer when
  // refactoring
  // TODO: Figure out some way to handle ordering because map doesn't care about
  // what folder was scanned first
  std::map<std::string, std::shared_ptr<const BlockDefinition>>
      _block_definitions;
  std::vector<std::string> _categories;
};
}  // namespace model
