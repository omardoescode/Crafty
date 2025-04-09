#pragma once

#include <filesystem>
#include <memory>
#include "block/block_definition.h"
#include "identity/id/id.h"
#include "project.h"
namespace model {
class ProjectManager {
public:
  constexpr static std::pair<float, float> world_resolution = {
      640, 320};  // https://www.youtube.com/shorts/y_TwL5IfPxA

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

  /**
   * @brief Add a character, whose initial sprite is given in `file_path`, by
   * creating an asset and putting it to `copy_folder`, and then attaching this
   * asset to the newly created character
   */
  std::shared_ptr<Character> add_character(std::filesystem::path file_path,
                                           std::filesystem::path copy_folder);

  /**
   * @brief Copy an asset from `file_path` to folder `copy_folder` and then
   * initiate this asset to asset_store
   */
  std::shared_ptr<Asset> add_asset(std::filesystem::path file_path,
                                   std::filesystem::path copy_folder);

  /**
   * @brief Create a script for character 'chr' with an initial bl ck definition
   * to create an instance of in the script and place at position (x, y)
   */
  std::shared_ptr<Script> add_script(std::shared_ptr<Character> chr,
                                     std::shared_ptr<const BlockDefinition> def,
                                     float x, float y);

  /**
   * @brief Create a block instance and add it to a script
   * @param id The script id to add this block to
   * @param definition The block definition to create an instance of
   * @param position The position to insert the instance at
   */
  void add_block_to_existing_script(IDPtr script_id, BlockDefPtr definition,
                                    int position);

  /**
   * @brief remove a character by using the id
   */
  void remove_character(IDPtr id);

  /**
   * @brief remove an asset by using the id
   */
  void remove_asset(IDPtr);

  /**
   * @brief remove a script by using the id
   */
  void remove_script(IDPtr);

  /**
   * @brief remove a block instance by using the id
   */
  void remove_block_instance(IDPtr);

  /**
   * @brief Get the Character sprite
   */
  std::shared_ptr<Asset> character_current_sprite(
      std::shared_ptr<Character> character);

  bool has_project() const;
  std::shared_ptr<Project> project() const;

private:
  ProjectManager();

private:
  std::shared_ptr<Project> _current_project;
};
}  // namespace model
