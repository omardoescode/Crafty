#pragma once
#include "character.h"
#include "events/event_dispatcher.h"
#include "script.h"

namespace model::events {
struct onCharacterCreated : public common::EventBase {
  std::shared_ptr<Character> character;
  onCharacterCreated(std::shared_ptr<Character> character)
      : character{character} {}
};

struct beforeCharacterDeleted : public common::EventBase {
  std::shared_ptr<Character> character;
  beforeCharacterDeleted(std::shared_ptr<Character> character)
      : character{character} {}
};
struct onScriptCreated : public common::EventBase {
  std::shared_ptr<Script> script;
  std::shared_ptr<Character> character;
  onScriptCreated(std::shared_ptr<Character> character,
                  std::shared_ptr<Script> script)
      : script{script}, character(character) {}
};
struct beforeScriptDeleted : public common::EventBase {
  std::shared_ptr<Script> script;
  beforeScriptDeleted(std::shared_ptr<Script> script) : script{script} {}
};
struct onBlockInstanceAddToScript : public common::EventBase {
  std::shared_ptr<Script> script;
  std::shared_ptr<BlockInstance> instance;
  int position;
  onBlockInstanceAddToScript(std::shared_ptr<Script> script,
                             std::shared_ptr<BlockInstance> instance,
                             int position)
      : script{script}, instance(instance), position(position) {}
};
}  // namespace model::events
