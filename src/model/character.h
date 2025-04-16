#pragma once
#include <vector>
#include "serialization/serializable.h"
namespace model {
class BlockInstance;

class Character : public Serializable {
public:
  Character(IDPtr id, float x, float y, float width, bool serialize = true);

  void add_sprite(IDPtr id, int pos = -1);
  void remove_sprite(IDPtr id);
  bool has_sprites() const;

  void add_script(IDPtr id, int pos = -1);
  void remove_script(IDPtr id);
  bool has_scripts() const;

  std::pair<float, float> pos() const;
  void set_pos(float x, float y);

  unsigned current_texture_idx() const;

  /**
   * @brief Retrieve the current texture
   * @return A shared pointer to the texture ID if exists or nullptr if there is
   * no current texture
   */
  IDPtr current_texture() const;

  void set_current_texture_idx(size_t);
  void next_texture();

  const std::vector<IDWPtr>& scripts() const;
  const std::vector<IDWPtr>& sprites() const;

  const std::string name() const;
  void set_name(const std::string& name);
  void set_name(std::string&& name);

  const float width() const;
  void set_width(float width);

private:
  std::vector<IDWPtr> _sprites;
  std::vector<IDWPtr> _scripts;
  std::pair<float, float> _pos;
  size_t _current_texture_idx;
  std::string _name;
  // The height is evaluated by multiplying the width by the ratio of height to
  // width in runtime
  float _width;
};
}  // namespace model
