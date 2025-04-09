#pragma once
#include <vector>
#include "identity/id/id.h"
#include "identity/serializable/serializable.h"
#include "script.h"
namespace model {
class BlockInstance;

class Character : public Serializable {
public:
  Character(IDPtr id, Project& project, float x, float y, float width);

  void add_sprite(IDPtr id, int pos = -1);
  void remove_sprite(IDPtr id);
  bool has_sprites() const;

  void add_script(IDPtr id, int pos = -1);
  void remove_script(IDPtr id);
  bool has_scripts() const;

  std::pair<float, float> pos() const;
  void set_pos(std::pair<float, float>);

  unsigned current_texture_idx() const;
  IDPtr current_texture() const;
  void set_current_texture_idx(size_t);
  void next_texture();

  const std::vector<IDPtr>& scripts() const;
  const std::vector<IDPtr>& sprites() const;

  const std::string name() const;
  void set_name(const std::string& name);
  void set_name(std::string&& name);

  const float width() const;
  void set_width(float width);

private:
  std::vector<IDPtr> _sprites;
  std::vector<IDPtr> _scripts;
  std::pair<float, float> _pos;
  size_t _current_texture_idx;
  std::string _name;
  // The height is evaluated by multiplying the width by the ratio of height to
  // width in runtime
  float _width;
};
}  // namespace model
