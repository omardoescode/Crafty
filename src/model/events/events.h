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
  std::shared_ptr<Script> instance;
  onScriptCreated(std::shared_ptr<Script> script) : instance{script} {}
};
}  // namespace model::events
