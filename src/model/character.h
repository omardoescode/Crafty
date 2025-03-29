#pragma once
#include <vector>
#include "script.h"
#include "utils/ID_manager.h"
#include "utils/serializable.h"
namespace model {
class BlockInstance;

class Character : public Serializable {
public:
  Character(const std::string& id, Project& project, float x, float y,
            float width);

  void add_sprite(const IDManager::IDType& id, int pos = -1);
  void remove_sprite(const IDManager::IDType& id);
  bool has_sprites() const;

  void add_script(const IDManager::IDType& id, int pos = -1);
  void remove_script(const IDManager::IDType& id);
  bool has_scripts() const;

  std::pair<float, float> pos() const;
  void set_pos(std::pair<float, float>);

  unsigned current_texture_idx() const;
  const IDManager::IDType& current_texture() const;
  void set_current_texture_idx(size_t);
  void next_texture();

  const std::vector<IDManager::IDType>& scripts() const;
  const std::vector<IDManager::IDType>& sprites() const;

  const std::string name() const;
  void set_name(const std::string& name);
  void set_name(std::string&& name);

  const float width() const;
  void set_width(float width);

private:
  std::vector<IDManager::IDType> _sprites;
  std::vector<IDManager::IDType> _scripts;
  std::pair<float, float> _pos;
  size_t _current_texture_idx;
  std::string _name;
  // The height is evaluated by multiplying the width by the ratio of height to
  // width in runtime
  float _width;
};
}  // namespace model
