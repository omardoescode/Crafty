#pragma once
#include <vector>
#include "serialization/serializable.h"
namespace model {
class BlockInstance;

class Character : public Serializable {
public:
  Character(IDPtr id, int x, int y, float width, bool serialize = true);

  /**
   * @brief Add a sprite id to the character
   * @param id The ID to add to sprite list
   * @param pos The position in sprite list
   * @note No checks to the validity of the ID
   */
  void add_sprite(IDPtr id, int pos = -1);

  /**
   * @brief Remove a specific sprite id
   * @note No checks to the validity of the ID
   */
  void remove_sprite(IDPtr id);

  /**
   * @brief check if the character has any sprites
   * @return `true` if the sprite list has any sprites
   */
  bool has_sprites() const;

  /**
   * @brief add a script to the script list
   * @param id the ID to add to script list
   * @param pos The position in script list
   * @note No checks to the validity of teh ID
   */
  void add_script(IDPtr id, int pos = -1);

  /**
   * @brief remove a specified script ID
   * @param id The id to remove
   */
  void remove_script(IDPtr id);

  /**
   * @brief Check if the character has any scripts
   * @return true if it has any scripts
   */
  bool has_scripts() const;

  /**
   * @brief Retrieve the current positon
   * @return a pair of x and y values that dictate position in world coordinates
   */
  std::pair<int, int> pos() const;

  /**
   * @brief Set a new position for the character
   * @param pos The (x,y) coordinates in world coordinates
   */
  void set_pos(std::pair<int, int> pos);

  /**
   * @brief Get the current sprite index in the sprite list
   * @return an index value
   */
  unsigned current_spirte_idx() const;

  /**
   * @brief Retrieve the current texture
   * @return A shared pointer to the texture ID if exists or nullptr if there is
   * no current texture
   */
  IDPtr current_sprite() const;

  /**
   * @brief Change the current sprite by changing the index
   * @param idx The new sprite index in the sprite list
   */
  void set_current_sprite_idx(size_t idx);

  /**
   * @brief Cycle through the sprite list and choose the next sprite in sprite
   * list as the new current
   */
  void next_sprite();

  /**
   * @brief Retrieve scripts
   * @return a vector of weak pointers to scripts
   */
  const std::vector<IDWPtr>& scripts() const;

  /**
   * @brief Retrieve sprites
   * @return a vector of weak pointers to sprites
   */
  const std::vector<IDWPtr>& sprites() const;

  /**
   * @brief Retrieve name
   * @return character name
   */
  const std::string name() const;

  /**
   * @brief Set name by lvalue
   * @return new name to set to character
   */
  void set_name(const std::string& name);

  /**
   * @brief Set name by rvalue
   * @return new name to set to character
   */
  void set_name(std::string&& name);

  /**
   * @brief Retreive width
   * @return Character Width
   */
  const float width() const;

  /**
   * @brief width setter
   * @param new_width new width value
   */
  void set_width(float new_width);

  /**
   * @brief Retreive Rotaion
   * @return Character rotation
   */
  int rotation() const;

  /**
   * @brief rotation setter
   * @param new_rotation new rotation value
   */
  void set_rotation(int new_rotation);

private:
  std::vector<IDWPtr> _sprites;
  std::vector<IDWPtr> _scripts;
  std::pair<int, int> _pos;
  size_t _current_texture_idx;
  std::string _name;
  // The height is evaluated by multiplying the width by the ratio of height to
  // width in runtime
  float _width;
  int _rotation;
};
}  // namespace model
