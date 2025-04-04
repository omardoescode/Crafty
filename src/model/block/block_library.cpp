#include "block/block_library.h"
#include <filesystem>
#include <memory>
#include <stdexcept>
#include "block/block_definition.h"
#include "block/block_instance.h"
#include "logger/logger.h"
#include "model_logger.h"
#include "utils/fs.h"
#include "utils/json.h"

namespace fs = std::filesystem;

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

  assert(fs::exists(initfile_path));
  // Read _initfile
  json initfile = parse_json(initfile_path);

  model_logger.info("Successfully initiated initfile");

  // Clear first
  _block_definitions.clear();

  // Start reading
  std::string data_directory = initfile.at("data_directory");
  fs::path path = construct_path(block_folder_pathname, data_directory.c_str());

  for (auto& category : initfile.at("categories")) {
    _categories.push_back(category.at("name"));

    std::string filename = category.at("name");
    filename += ".json";

    fs::path category_path = path;
    category_path /= filename;

    assert(fs::exists(category_path));
    model_logger.error("Reading category path: " + category_path.string());
    json category_file = parse_json(category_path);

    for (auto& blck : category_file) {
      BlockDefinition def = _parse_block(blck, category.at("name"));
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
  for (const auto& [id, def] : _block_definitions) {
    if (def->category() == category) res.push_back(def);
  }

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
