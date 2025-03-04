#pragma once
#include "BlockDefinition.h"
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

private:
  BlockLibrary();

  void _load_blocks();
  void _load_blocks_initfile();
  void _load_blocks_json_data();
  BlockDefinition _parse_block(const json&, const std::string&);

private:
  bool _initialized;
  json _initfile;
  std::map<std::string, BlockDefinition> _block_definitions;
};
}  // namespace model
