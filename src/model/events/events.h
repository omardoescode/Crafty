#pragma once
#include "character.h"
#include "events/event_dispatcher.h"

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
}  // namespace model::events
