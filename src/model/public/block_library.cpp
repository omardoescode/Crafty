#include "block_library.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "block/block_definition.h"
#include "utils/fs.h"
#define MODEL_LAYER_DEBUG
namespace model {
const char* BlockLibrary::block_folder_pathname = "./data/blocks";
const char* BlockLibrary::block_initfile_pathname = "__init__.json";

BlockLibrary::BlockLibrary() : _initialized(false) {}

BlockLibrary& BlockLibrary::instance() {
  static BlockLibrary _instance;
  return _instance;
}

void BlockLibrary::initialize() {
  if (_initialized) return;
  _load_blocks();
  _initialized = true;
}

void BlockLibrary::reload() { _load_blocks(); }

void BlockLibrary::_load_blocks() {
  _load_blocks_initfile();
  _load_blocks_json_data();
}

void BlockLibrary::_load_blocks_initfile() {
  auto path = construct_path(block_folder_pathname, block_initfile_pathname);

  // Read the file
  std::cout << "Successl" << std::endl;
  _initfile = parse_json(path);
}
void BlockLibrary::_load_blocks_json_data() {
  // Clear first
  _block_definitions.clear();

  // Start reading
  std::string data_directory = _initfile.at("data_directory");
  auto path = construct_path(block_folder_pathname, data_directory.c_str());

  for (std::filesystem::path const& dir_entry :
       std::filesystem::directory_iterator{path}) {
    assert(std::filesystem::is_directory(dir_entry));
    _categories.push_back(dir_entry.filename());

#ifdef MODEL_LAYER_DEBUG
    std::cout << "Reading Category " << dir_entry << std::endl;
#endif
    for (std::filesystem::path const& block_definition :
         std::filesystem::directory_iterator(dir_entry)) {
      assert(std::filesystem::is_regular_file(block_definition));
#ifdef MODEL_LAYER_DEBUG
      std::cout << "Reading block " << block_definition << std::endl;
#endif

      std::fstream file(block_definition);
      json block_def_json = json::parse(file);
      BlockDefinition def = _parse_block(block_def_json, dir_entry.filename());
      if (_block_definitions.count(def.id) != 0)
        throw std::runtime_error("Two blocks have the same ID");
      _block_definitions[def.id] = std::make_shared<BlockDefinition>(def);
    }
  }

#ifdef MODEL_LAYER_DEBUG
  std::cout << "Finished Reading " << _block_definitions.size()
            << " builtin blocks" << std::endl;
#endif
}

BlockDefinition BlockLibrary::_parse_block(const json& js,
                                           const std::string& category) {
  BlockDefinition def;
  js.at("id").get_to(def.id);
  js.at("name").get_to(def.name);
  js.at("has_body").get_to(def.has_body);
  def.category = category;

  static const std::unordered_map<std::string, SlotType> typeMap = {
      {"Number", SlotType::Number},
      {"Exec", SlotType::Exec},
      {"Boolean", SlotType::Boolean},
      {"String", SlotType::String},
      {"Any", SlotType::Any}};
  // Parse input slots
  for (auto& slot : js.at("inputs")) {
    InputSlot parsed_slot;
    parsed_slot.type = typeMap.at(slot.at("type"));
    parsed_slot.label = slot.at("label");
    parsed_slot.default_value = slot["default_value"];
  }

  // Parse input slot type
  def.output.type = typeMap.at((std::string)js.at("output").at("type"));

  return def;
}

const std::vector<std::string>& BlockLibrary::categories() const {
  assert(_initialized);
  return _categories;
}

std::vector<std::shared_ptr<const BlockDefinition>>
BlockLibrary::category_blocks(const std::string& category) const {
  std::vector<std::shared_ptr<const BlockDefinition>> res;
  for (const auto& [id, def] : _block_definitions) {
    if (def->category == category) res.push_back(def);
  }

  return res;
}

std::shared_ptr<const BlockDefinition> BlockLibrary::get_block_definition_by_id(
    const BlockDefinitionID& id) const {
  if (_block_definitions.count(id) == 0)
    throw std::runtime_error("Invalid ID, or ID doesn't exist in library");
  return _block_definitions.at(id);
}
}  // namespace model
