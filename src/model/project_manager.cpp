#include "project_manager.h"
#include <cassert>
#include <filesystem>
#include <memory>
#include <random>
#include "asset.h"
#include "block/block_definition.h"
#include "block/block_instance.h"
#include "block/block_library.h"
#include "block/json_block_storage.h"
#include "character.h"
#include "events/event_dispatcher.h"
#include "model_events.h"
#include "model_logger.h"
#include "utils/fs.h"

namespace fs = std::filesystem;
std::random_device rd;
std::default_random_engine gen(rd());

auto& dispatcher = common::EventDispatcher::instance();

namespace model {
ProjectManager::ProjectManager() : _current_project(nullptr) {
  // ID generator for storage
  BlockLibrary::Config config;
  config.block_file_path = "./data/blocks.json";
  _block_lib = std::make_shared<BlockLibrary>(
      std::make_unique<JsonBlockStorage>(), config);

  model_logger().info("Project Manager Initialized");
}

ProjectManager& ProjectManager::instance() {
  static ProjectManager instance;
  return instance;
}

void ProjectManager::create() {
  assert(!_current_project && "There is a project already");
  _current_project = std::make_shared<Project>("Untitled");
}

void ProjectManager::save() {
  assert(_current_project && "There is no current project");
  // TODO: implement a serialization engine and use it here
}

void ProjectManager::load(const fs::path& path) {
  assert(!_current_project && "There is a project already");
  assert(path.extension() == "crafty");
  // TODO: implement a serialization engine and use it here
}

void ProjectManager::clear() {
  assert(_current_project && "There's no project to clear");
  _current_project.reset();
}

bool ProjectManager::has_project() const { return !!_current_project; }

std::shared_ptr<Project> ProjectManager::project() const {
  return _current_project;
}
std::string getCurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_c), "%Y%m%d%H%M%S");
  return ss.str();
}

std::shared_ptr<Asset> ProjectManager::add_asset(
    std::shared_ptr<Character> character, std::filesystem::path file_path,
    std::filesystem::path copy_folder) {
  assert(_current_project && "There's no current project");
  assert(fs::exists(file_path) && "File path doesn't exist");
  assert(fs::exists(copy_folder) && "Copy folder doesn't exist");
  assert(fs::is_regular_file(file_path) && "File path is not a file");
  assert(fs::is_directory(copy_folder) && "Copy folder is not a directory");

  // copy the file
  std::string extension = file_path.extension();
  fs::path destination = copy_folder / generate_unique_filename(extension);
  fs::copy(file_path, destination);

  // Create a new entity
  auto new_asset = std::make_shared<Asset>(destination);

  character->add_sprite(new_asset);
  return new_asset;
}

std::shared_ptr<Character> ProjectManager::add_character(
    std::filesystem::path file_path, std::filesystem::path copy_folder) {
  assert(_current_project && "There's no current project");
  assert(fs::exists(file_path) && "File path doesn't exist");
  assert(fs::exists(copy_folder) && "Copy folder doesn't exist");
  assert(fs::is_regular_file(file_path) && "File path is not a file");
  assert(fs::is_directory(copy_folder) && "Copy folder is not a directory");

  // Get random coordinates
  int x_margin = world_resolution.first / 4;
  int y_margin = world_resolution.second / 4;

  std::uniform_int_distribution<int> x_pos_gen(
      x_margin, world_resolution.first - x_margin);
  std::uniform_int_distribution<int> y_pos_gen(
      y_margin, world_resolution.second - y_margin);

  auto new_char =  // TODO: FIND a solution to the name problem? an injected
                   // solution is preferable
      std::make_shared<Character>("Name??", x_pos_gen(gen), y_pos_gen(gen),
                                  100);  // TODO: Refactor this magic number
  std::shared_ptr<Asset> new_asset =
      add_asset(new_char, file_path, copy_folder);
  new_char->add_sprite(new_asset);

  _current_project->add_character(new_char);

  dispatcher.publish(std::make_shared<events::onCharacterCreated>(new_char));

  return new_char;
}

std::shared_ptr<Script> ProjectManager::add_script(
    std::shared_ptr<Character> chr, BlockDefPtr def, float x, float y) {
  auto script = std::make_shared<Script>(x, y);
  auto instance = std::make_shared<BlockInstance>(def);

  script->add_block_instance(instance);
  chr->add_script(script);
  dispatcher.publish(std::make_shared<events::onScriptCreated>(chr, script));

  return script;
}

void ProjectManager::add_block_to_existing_script(
    std::shared_ptr<Script> script, BlockDefPtr def, int position) {
  // Make sure it's not a starter
  // NOTE: The only was is to create a script with it. nothing else
  // TODO: MOVE FROM HERE
  if (def->is_starter()) return;

  // Get Script

  // Create the instance first and publish it
  auto instance = std::make_shared<BlockInstance>(def);

  int final_position = script->add_block_instance(instance, position);

  if (final_position != -1)
    // Publish the event
    dispatcher.publish(std::make_shared<events::onBlockInstanceAddToScript>(
        script, instance, final_position));
}

std::shared_ptr<BlockLibrary> ProjectManager::block_lib() const {
  return _block_lib;
}
}  // namespace model
