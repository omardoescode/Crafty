#pragma once
#include <asset.h>
#include <script.h>
#include <memory>
#include <string>
#include <vector>
#include "serialization/serializable.h"
namespace model {
class BlockInstance;

class Character : public Serializable {
public:
  Character(const std::string& name, int x, int y, float width);
  Character(std::string&& name, int x, int y, float width);

  /**
   * @brief Add a sprite id to the character
   * @param sprite The asset to add
   * @param pos The position in sprite list
   */
  void add_sprite(std::shared_ptr<Asset> sprite, int pos = -1);

  /**
   * @brief Remove a specific sprite
   */
  void remove_sprite(std::shared_ptr<Asset> asset);

  /**
   * @brief check if the character has any sprites
   * @return `true` if the sprite list has any sprites
   */
  bool has_sprites() const;

  /**
   * @brief add a script to the script list
   * @param script The script to add
   * @param pos The position in script list
   */
  void add_script(std::shared_ptr<Script> script, int pos = -1);

  /**
   * @brief remove a specified script
   */
  void remove_script(std::shared_ptr<Script> script);

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
   * @return A shared pointer to the current sprite
   */
  std::shared_ptr<Asset> current_sprite() const;

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
   * @return a vector of to scripts
   */
  const std::vector<std::shared_ptr<Script>>& scripts() const;

  /**
   * @brief Retrieve sprites
   * @return a vector of sprites
   */
  const std::vector<std::shared_ptr<Asset>>& sprites() const;

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
  std::vector<std::shared_ptr<Asset>> _sprites;
  std::vector<std::shared_ptr<Script>> _scripts;
  std::pair<int, int> _pos;
  size_t _current_texture_idx;
  std::string _name;
  // The height is evaluated by multiplying the width by the ratio of height to
  // width in runtime
  float _width;
  int _rotation;
};
}  // namespace model
