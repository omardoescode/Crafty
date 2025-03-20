#pragma once
#include <memory>
#include <vector>
namespace model {
class Character;
class BlockInstance;

class Script {
public:
  typedef std::string ScriptID;

public:
  /**
   * @brief A constructor with the the initial block for the script and the
   * character to create
   */

  Script(const ScriptID& id, std::shared_ptr<Character>,
         std::shared_ptr<BlockInstance>);

  void add_block(std::shared_ptr<BlockInstance>, int pos = -1);
  void remove_block(int pos);
  bool empty_script() const;

  const ScriptID& id() const;
  const std::vector<std::shared_ptr<BlockInstance>>& blocks() const;
  std::shared_ptr<Character> character();

private:
  std::vector<std::shared_ptr<BlockInstance>> _blocks;
  std::shared_ptr<Character> _character;
  ScriptID _id;
};
}  // namespace model
