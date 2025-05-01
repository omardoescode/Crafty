#pragma once
#include "events/event_base.h"
#include "script.h"
namespace logic::events {
struct onScriptFinishedExecution : public common::EventBase {
  std::weak_ptr<model::Script> script;
  onScriptFinishedExecution(std::weak_ptr<model::Script> script)
      : script(script) {}
};
struct LuaGameEvent : public common::EventBase {
  std::string event_name;
  LuaGameEvent(std::string event_name) : event_name(event_name) {}
};
};  // namespace logic::events
