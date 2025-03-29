#include "project_manager.h"
#include <cassert>
#include <filesystem>
#include <memory>
#include <random>
#include "character.h"
#include "events/event_dispatcher.h"
#include "events/events.h"
#include "utils/ID_manager.h"
#include "utils/fs.h"

namespace fs = std::filesystem;
std::random_device rd;
std::default_random_engine gen(rd());

namespace model {
ProjectManager::ProjectManager() : _current_project(nullptr) {}

ProjectManager& ProjectManager::instance() {
  static ProjectManager instance;
  return instance;
}

void ProjectManager::create() {
  assert(!_current_project && "There is a project already");
  auto char_mgr_id = std::make_unique<IDManager>("char");
  auto char_store = std::make_unique<Store<Character>>(std::move(char_mgr_id));

  auto script_mgr_id = std::make_unique<IDManager>("script");
  auto script_store = std::make_unique<Store<Script>>(std::move(script_mgr_id));

  auto asset_mgr_id = std::make_unique<IDManager>("asset");
  auto asset_store = std::make_unique<Store<Asset>>(std::move(asset_mgr_id));

  auto instances_mgr_id = std::make_unique<IDManager>("instances");
  auto instances_store =
      std::make_unique<Store<BlockInstance>>(std::move(instances_mgr_id));

  _current_project = std::make_shared<Project>(
      untitled_project_name, std::move(char_store), std::move(script_store),
      std::move(asset_store), std::move(instances_store));
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
std::shared_ptr<Asset> ProjectManager::add_asset(fs::path file_path,
                                                 fs::path copy_folder) {
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
  std::shared_ptr<Asset> new_asset =
      _current_project->asset_store().create_entity(*_current_project,
                                                    destination.string());
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

  std::shared_ptr<Asset> new_asset = add_asset(file_path, copy_folder);
  std::shared_ptr<Character> new_char =
      _current_project->char_store().create_entity(
          *_current_project, x_pos_gen(gen), y_pos_gen(gen),
          100);  // TODO: Refactor this magic number
  new_char->add_sprite(new_asset->id());

  auto& dispatcher = common::EventDispatcher::instance();
  dispatcher.publish(std::make_shared<events::onCharacterCreated>(new_char));

  return new_char;
}

std::shared_ptr<Asset> ProjectManager::character_current_sprite(
    std::shared_ptr<Character> character) {
  auto asset_id = character->current_texture();
  auto asset = _current_project->asset_store().get_entity(asset_id);
  return asset;
}

void ProjectManager::remove_character(const IDManager::IDType& character_id) {
  auto chr = _current_project->char_store().get_entity(character_id);
  auto& dispatcher = common::EventDispatcher::instance();
  dispatcher.publish(std::make_shared<events::beforeCharacterDeleted>(chr));

  // remove all sprites & scripts
  for (auto& sprite : chr->sprites()) {
    remove_asset(sprite);
  }
  for (auto& script : chr->scripts()) {
    remove_script(script);
  }

  _current_project->char_store().remove_entity(character_id);

  // The remaining reference is in this scope only, which will be deleted
  // before this
  assert(chr.use_count() == 1 &&
         "There are still remaining references to this object");
}

void ProjectManager::remove_asset(const IDManager::IDType& asset_id) {
  _current_project->asset_store().remove_entity(asset_id);
}

void ProjectManager::remove_script(const IDManager::IDType& script_id) {
  auto scr = _current_project->script_store().get_entity(script_id);
  for (auto& blk : scr->blocks()) remove_block_instance(blk);
  _current_project->asset_store().remove_entity(script_id);
}

void ProjectManager::remove_block_instance(
    const IDManager::IDType& instance_id) {
  auto blk = _current_project->instances_store().get_entity(instance_id);

  // if has body, remove it
  if (blk->has_body()) remove_block_instance(blk->body());

  // remove
  _current_project->instances_store().remove_entity(instance_id);
}
}  // namespace model
