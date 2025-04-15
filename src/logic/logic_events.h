#pragma once
#include "events/event_base.h"
#include "script.h"
namespace logic::events {
struct onScriptFinishedExecution : public common::EventBase {
  std::weak_ptr<model::Script> script;
  onScriptFinishedExecution(std::weak_ptr<model::Script> script)
      : script(script) {}
};
};  // namespace logic::events
