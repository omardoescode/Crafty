#include "project.h"
#include <cassert>

namespace model {
Project::Project(const std::string& title,
                 std::unique_ptr<Store<Character>> char_store,
                 std::unique_ptr<Store<Script>> script_store,
                 std::unique_ptr<Store<Asset>> asset_store,
                 std::unique_ptr<Store<BlockInstance>> instances_store)
    : _title(title),
      _char_store(std::move(char_store)),
      _script_store(std::move(script_store)),
      _asset_store(std::move(asset_store)),
      _instances_store(std::move(instances_store)) {
  assert(_char_store);
  assert(_script_store);
  assert(_asset_store);
  assert(_instances_store);
}

Project::Project(std::string&& title,
                 std::unique_ptr<Store<Character>> char_store,
                 std::unique_ptr<Store<Script>> script_store,
                 std::unique_ptr<Store<Asset>> asset_store,
                 std::unique_ptr<Store<BlockInstance>> instances_store)
    : _title(std::move(title)),
      _char_store(std::move(char_store)),
      _script_store(std::move(script_store)),
      _asset_store(std::move(asset_store)),
      _instances_store(std::move(instances_store)) {
  assert(_char_store);
  assert(_script_store);
  assert(_asset_store);
  assert(_instances_store);
}

Store<Character>& Project::char_store() const { return *_char_store; }
Store<Script>& Project::script_store() const { return *_script_store; }
Store<Asset>& Project::asset_store() const { return *_asset_store; }
Store<BlockInstance>& Project::instances_store() const {
  return *_instances_store;
}
}  // namespace model
