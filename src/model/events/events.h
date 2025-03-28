#pragma once
#include "character.h"
#include "events/event_dispatcher.h"

namespace model::events {
struct onCharacterCreated : public common::EventBase {
  std::shared_ptr<Character> chr;
  onCharacterCreated(std::shared_ptr<Character> chr) : chr{chr} {}
};
}  // namespace model::events
