#pragma once
#include <sol/sol.hpp>
#include <string>
#include "events/event_base.h"
class LuaGameEvent : public common::EventBase {
public:
  std::string event_name;
  sol::table data;

  LuaGameEvent() = default;
  LuaGameEvent(std::string name, sol::table data)
      : event_name(std::move(name)), data(std::move(data)) {}
};
