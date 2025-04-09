#include "json_block_storage.h"
#include <stdexcept>
#include "block/block_definition.h"
#include "model_logger.h"
#include "utils/json.h"

namespace model {

JsonBlockStorage::JsonBlockStorage(IDGeneratorPtr id_generator)
    : _id_generator(std::move(id_generator)) {}

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
      // Create Input Slots
      std::vector<InputSlot> inputs;
      for (auto& slot : block.at("inputs"))
        inputs.emplace_back(
            InputSlot{.label = slot.at("label"),
                      .type = parse_type(slot.at("type")),
                      .default_value = slot.at("default_value")});

      // Create the output slot
      OutputSlot output;
      output.type = parse_type(block.at("output").at("type"));

      // Extract Other data
      std::string name = block.at("name");
      bool has_body = block.at("has_body");
      int options = 0;
      if (has_body) options |= BlockDefinition::BLOCKDEF_HASBODY;

      // Create the block definition
      BlockDefPtr def = std::make_shared<BlockDefinition>(
          name, category, std::move(inputs), std::move(output));

      // Create an id and put the values in the maps
      IDPtr id = _id_generator->generate_next();
      _defs[id] = def;
      _category_defs[std::move(category)].push_back(def);
    }
  }

  model_logger.info("Loaded {} block definitions\n", _defs.size());
}

std::vector<std::string> JsonBlockStorage::categories() const {
  std::vector<std::string> categories;
  categories.reserve(_category_defs.size());
  for (auto& [category, _] : _category_defs) categories.push_back(category);
  return categories;
}

BlockDefPtr JsonBlockStorage::get_definition_by_id(const IDPtr& id) const {
  return _defs.at(id);
}
std::vector<BlockDefPtr> JsonBlockStorage::get_definitions_by_category(
    const std::string& category) const {
  return _category_defs.at(category);
}
}  // namespace model
