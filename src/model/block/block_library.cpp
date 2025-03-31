#include "block/block_library.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include "block/block_definition.h"
#include "block/block_instance.h"
#include "logger/logger.h"
#include "model_logger.h"
#include "utils/fs.h"
#include "utils/json.h"
namespace model {
BlockDefinition _parse_block(const json& js, const std::string& category);

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

void BlockLibrary::load_project(std::shared_ptr<Project> proj) {
  assert(proj);
  _project = proj;
}
void BlockLibrary::reload() { _load_blocks(); }

void BlockLibrary::_load_blocks() {
  auto initfile_path =
      construct_path(block_folder_pathname, block_initfile_pathname);

  std::filesystem::path p(".");
  assert(std::filesystem::exists(initfile_path));
  // Read _initfile
  json initfile = parse_json(initfile_path);

  model_logger.info("Successfully initiated initfile");

  // Clear first
  _block_definitions.clear();

  // Start reading
  std::string data_directory = initfile.at("data_directory");
  auto path = construct_path(block_folder_pathname, data_directory.c_str());

  for (std::filesystem::path const& dir_entry :
       std::filesystem::directory_iterator{path}) {
    assert(std::filesystem::is_directory(dir_entry));
    _categories.push_back(dir_entry.filename());

    model_logger.info("Reading Category: " + dir_entry.string());
    for (std::filesystem::path const& block_definition :
         std::filesystem::directory_iterator(dir_entry)) {
      assert(std::filesystem::is_regular_file(block_definition));

      model_logger.info("Reading Block: " + block_definition.string());

      std::fstream file(block_definition);
      json block_def_json = json::parse(file);
      BlockDefinition def = _parse_block(block_def_json, dir_entry.filename());
      if (_block_definitions.count(def.id()) != 0)
        throw std::runtime_error("Two blocks have the same ID");
      _block_definitions[def.id()] = std::make_shared<BlockDefinition>(def);
    }
  }
}

BlockDefinition _parse_block(const json& js, const std::string& category) {
  static const std::unordered_map<std::string, BlockDefinition::SlotType>
      typeMap = {{"Number", BlockDefinition::SlotType::Number},
                 {"Exec", BlockDefinition::SlotType::Exec},
                 {"Boolean", BlockDefinition::SlotType::Boolean},
                 {"String", BlockDefinition::SlotType::String},
                 {"Any", BlockDefinition::SlotType::Any}};
  // Parse input slots
  std::vector<BlockDefinition::InputSlot> input_slots;
  for (auto& slot : js.at("inputs")) {
    BlockDefinition::InputSlot parsed_slot;
    parsed_slot.type = typeMap.at(slot.at("type"));
    parsed_slot.label = slot.at("label");
    parsed_slot.default_value = slot["default_value"];
    input_slots.push_back(parsed_slot);
  }

  decltype(BlockDefinition::OutputSlot::type) output =
      typeMap.at((std::string)js.at("output").at("type"));

  int options = 0;
  if (js.at("has_body")) options |= BlockDefinition::BLOCKDEF_HASBODY;

  BlockDefinition def(js.at("id"), js.at("name"), category,
                      std::move(input_slots), {.type = output}, options);
  return def;
}

const std::vector<std::string>& BlockLibrary::categories() const {
  assert(_initialized);
  return _categories;
}

std::vector<std::shared_ptr<const BlockDefinition>>
BlockLibrary::category_blocks(const std::string& category) const {
  std::vector<std::shared_ptr<const BlockDefinition>> res;
  for (const auto& [id, def] : _block_definitions)
    if (def->category() == category) res.push_back(def);

  return res;
}

std::shared_ptr<const BlockDefinition> BlockLibrary::get_block_definition_by_id(
    const BlockLibrary::BlockDefIDType& id) const {
  if (_block_definitions.count(id) == 0)
    throw std::runtime_error("Invalid ID, or ID doesn't exist in library");
  return _block_definitions.at(id);
}

std::shared_ptr<BlockInstance> BlockLibrary::create_dummy_instance(
    const BlockDefIDType& id) {
  assert(_project && "Project doesn't exist");
  auto itr = _block_definitions.find(id);
  assert(itr != _block_definitions.end() && "ID not found in lib");
  return std::make_shared<BlockInstance>(BlockInstance::DUMMY_INSTANCE_ID,
                                         *_project, itr->second, -1, -1);
}
}  // namespace model
