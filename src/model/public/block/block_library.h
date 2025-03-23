#pragma once
#include <map>
#include <memory>
#include <vector>
namespace model {
class BlockDefinition;
class BlockInstance;
class Project;
class BlockLibrary {
public:
  constexpr static std::string DUMMY_INSTANCE_ID = "";
  static const char* block_folder_pathname;
  static const char* block_initfile_pathname;

  typedef std::string BlockDefIDType;

public:
  // remove copying for singltons
  BlockLibrary(BlockLibrary&) = delete;
  BlockLibrary& operator=(BlockLibrary&) = delete;

  // Get instance
  static BlockLibrary& instance();

  // Initialize the block library
  void initialize();

  void load_project(std::shared_ptr<Project>);

  void reload();

  const std::vector<std::string>& categories() const;
  std::vector<std::shared_ptr<const BlockDefinition>> category_blocks(
      const std::string& category) const;

  std::shared_ptr<const BlockDefinition> get_block_definition_by_id(
      const BlockDefIDType&) const;

  std::shared_ptr<BlockInstance> create_dummy_instance(
      const BlockDefIDType& id);

private:
  BlockLibrary();

  void _load_blocks();

private:
  bool _initialized;
  // TODO: Optimize category block queries by changing this design layer when
  // refactoring
  // TODO: Figure out some way to handle ordering because map doesn't care about
  // what folder was scanned first
  std::map<std::string, std::shared_ptr<const BlockDefinition>>
      _block_definitions;
  std::vector<std::string> _categories;
  std::shared_ptr<Project> _project;
};
}  // namespace model
