#include "json_block_storage.h"
#include <exception>
#include <stdexcept>
#include <string>
#include "block/block_definition.h"
#include "block/value_type.h"
#include "input_slot.h"
#include "model_logger.h"
#include "utils/json.h"

namespace model {

JsonBlockStorage::JsonBlockStorage() {}

void JsonBlockStorage::load_definitions(const std::filesystem::path& path) {
  // Parse The file
  json file;
  try {
    file = parse_json(path);
  } catch (std::exception&) {
    throw std::runtime_error(
        "JsonBlockStorage::load_definitions - Failed to parse JSON");
  }

  // Read The defintions
  for (auto& element : file) {
    std::string category = element["name"].get<std::string>();

    for (auto& block : element.at("blocks")) {
      // try {
      // Create Input Slots
      std::vector<InputSlotDef> inputs;
      for (auto& slot : block.at("inputs")) {
        std::string type = slot.at("type");
        Value value(to_value_type(type));
        switch (value.type()) {
          case ValueType::NUMBER:
            value.set((int)slot.at("default_value"));
            break;
          case ValueType::TEXT:
            value.set((std::string)slot.at("default_value"));
            break;
          default:
            throw model_logger().error("Input slot of invalid type: {}",
                                       static_cast<int>(value.type()));
            break;
        }
        inputs.emplace_back(slot.at("label"), value);
      }

      // Create the output slot
      OutputSlot output;
      output.type = to_value_type(block.at("output").at("type"));

      // Extract Other data
      std::string name = block.at("name");
      std::string id = block.at("id");

      // Check for optional data
      int options = BlockDefinition::BLOCKDEF_DEFAULT;
      if (block.contains("has_body") && block["has_body"].is_boolean())
        options |= BlockDefinition::BLOCKDEF_HASBODY;
      if (block.contains("starter") && block["starter"].is_boolean())
        options |= BlockDefinition::BLOCKDEF_STARTER;

      // Create the ID and the the block definition
      BlockDefPtr def = std::make_shared<BlockDefinition>(
          id, name, category, std::move(inputs), std::move(output), options);

      // put the values in the maps
      _defs.emplace(id, def);
      _category_defs[category].push_back(def);
      // } catch (const std::exception& exx) {
      //   model_logger().warn("Failed to load block");
      // }
    }

    model_logger().info("Loaded {} block definitions of category {}",
                        _defs.size(), category);
  }

  model_logger().info("Loaded {} block definitions", _defs.size());
}

std::vector<std::string> JsonBlockStorage::categories() const {
  std::vector<std::string> categories;
  categories.reserve(_category_defs.size());
  for (auto& [category, _] : _category_defs) categories.push_back(category);
  return categories;
}

BlockDefPtr JsonBlockStorage::get_definition_by_id(std::string id) const {
  return _defs.at(id);
}
std::vector<BlockDefPtr> JsonBlockStorage::get_definitions_by_category(
    const std::string& category) const {
  auto itr = _category_defs.find(category);
  if (itr == _category_defs.end()) return {};
  return itr->second;
}
}  // namespace model
