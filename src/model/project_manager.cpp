#include "project_manager.h"
#include <cassert>
#include <memory>

namespace model {

ProjectManager::ProjectManager() : _current_project(nullptr) {}

ProjectManager& ProjectManager::instance() {
  static ProjectManager instance;
  return instance;
}

void ProjectManager::create() {
  assert(!_current_project && "There is a project already");
  _current_project = std::make_shared<Project>(_untitled_project_name);
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
