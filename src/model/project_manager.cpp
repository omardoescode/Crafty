#include "project_manager.h"
#include <cassert>
#include <memory>
#include "utils/ID_manager.h"

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

void ProjectManager::load(const std::filesystem::path& path) {
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

}  // namespace model
