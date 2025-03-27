#pragma once

#include <memory>
#include "utils/store.h"

#include "asset.h"
#include "block/block_instance.h"
#include "character.h"
#include "script.h"
namespace model {

class Project {
public:
  Project(const std::string& title, std::unique_ptr<Store<Character>>,
          std::unique_ptr<Store<Script>>, std::unique_ptr<Store<Asset>>,
          std::unique_ptr<Store<BlockInstance>>);
  Project(std::string&& title, std::unique_ptr<Store<Character>>,
          std::unique_ptr<Store<Script>>, std::unique_ptr<Store<Asset>>,
          std::unique_ptr<Store<BlockInstance>>);

  Store<Character>& char_store() const;
  Store<Script>& script_store() const;
  Store<Asset>& asset_store() const;
  Store<BlockInstance>& instances_store() const;

private:
  std::string _title;
  std::unique_ptr<Store<Character>> _char_store;
  std::unique_ptr<Store<Script>> _script_store;
  std::unique_ptr<Store<Asset>> _asset_store;
  std::unique_ptr<Store<BlockInstance>> _instances_store;
};
}  // namespace model
