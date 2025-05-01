#include "project.h"
#include <cassert>
#include <memory>
#include "events/event_dispatcher.h"
#include "model_events.h"
#include "model_logger.h"

namespace model {
Project::Project(const std::string& name,
                 std::vector<std::shared_ptr<Character>> character_list)
    : _character_list(character_list), _name(name), _is_dirty(false) {}

const std::vector<std::shared_ptr<Character>>& Project::characters() const {
  return _character_list;
}

const std::string& Project::name() const { return _name; }
void Project::set_name(const std::string& new_name) { _name = new_name; }

void Project::mark_dirty() { _is_dirty = true; }
void Project::mark_undirty() { _is_dirty = false; }
bool Project::is_dirty() const { return _is_dirty; }

void Project::add_character(std::shared_ptr<Character> chr) {
  _character_list.push_back(chr);
}
void Project::remove_character(std::shared_ptr<Character> chr) {
  auto& dispatcher = common::EventDispatcher::instance();
  dispatcher.publish(std::make_shared<events::beforeCharacterDeleted>(chr));
  _character_list.erase(
      std::find(_character_list.begin(), _character_list.end(), chr));
  model_logger().info("Removing Character {}", _name);
}
}  // namespace model
