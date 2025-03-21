#pragma once

#include <filesystem>
#include "project.h"
namespace model {
class ProjectManager {
public:
  constexpr static std::string _untitled_project_name = "Untitled";

public:
  // remove copying
  ProjectManager(ProjectManager&) = delete;
  ProjectManager& operator=(ProjectManager&) = delete;

  static ProjectManager& instance();

  /**
   * Create a new project and set as the current project
   */
  void create();

  /**
   * Given the path to a `.crafty` file, use it to reconstruct the project data
   */
  void load(const std::filesystem::path&);

  /**
   * Use the current project data. If not called, all the edits in the project
   * would be wasted
   */
  void save();

  /**
   * Clear the current project
   */
  void clear();

  bool has_project() const;
  std::shared_ptr<Project> project() const;

private:
  ProjectManager();

private:
  std::shared_ptr<Project> _current_project;
};
}  // namespace model
