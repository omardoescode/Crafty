#include "project_manager.h"
#include <cassert>
#include <filesystem>
#include <memory>
#include "character.h"
#include "utils/ID_manager.h"
#include "utils/fs.h"

namespace fs = std::filesystem;

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
      _untitled_project_name, std::move(char_store), std::move(script_store),
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

  std::shared_ptr<Asset> new_asset = add_asset(file_path, copy_folder);
  std::shared_ptr<Character> new_char =
      _current_project->char_store().create_entity(*_current_project);
  new_char->add_sprite(new_asset->id());
  return new_char;
}
}  // namespace model
