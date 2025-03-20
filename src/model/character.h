#pragma once
#include <map>
#include <memory>
#include <vector>
#include "asset.h"
#include "script.h"
namespace model {
class BlockInstance;
class Character {
public:
  typedef std::string CharacterID;

public:
  Character(const std::string& id);
  void add_texture(std::shared_ptr<Asset>, int pos = -1);
  void remove_texture(int);
  bool no_textures() const;

  void add_script(std::shared_ptr<Script>);
  void remove_script(const Script::ScriptID& id);
  std::pair<float, float> pos() const;
  void set_pos(std::pair<float, float>);

  unsigned current_texture_idx() const;
  std::shared_ptr<Asset> current_texture() const;
  void set_current_texture_idx(int i);
  void next_texture();

  const std::string& id() const;

  const std::map<Script::ScriptID, std::shared_ptr<Script>>& scripts() const;

private:
  std::vector<std::shared_ptr<Asset>> _textures;
  std::map<Script::ScriptID, std::shared_ptr<Script>> _scripts;
  std::pair<float, float> _pos;
  unsigned _current_texture_idx;
  CharacterID _id;
};
}  // namespace model
