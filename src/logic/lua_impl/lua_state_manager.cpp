#include "lua_impl/lua_state_manager.h"
#include <memory>
#include <thread>
#include "character.h"
#include "events/event_dispatcher.h"
#include "game_events.h"
#include "logic_logger.h"
#include "lua_impl/lua_module_cache.h"
#include "lua_impl/thread_pool.h"
#include "project_manager.h"
#include "scope_table.h"

namespace logic::lua {
sol::state& LuaStateManager::state() { return _state; }

void LuaStateManager::initialize(LuaModuleCache& module_cache) {
  _state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);

  auto& mgr = model::ProjectManager::instance();

  auto categories = mgr.block_lib()->categories();
  for (auto& category : categories) {
    _state.require_script(category, module_cache.get_module(category));
  }

  // TODO: Fix This mess
  _state["debug_args"] = [](sol::variadic_args va) {
    std::cout << "Number of arguments: " << va.size() << std::endl;
    int idx = 0;
    for (auto&& v : va) {
      std::cout << "Arg " << idx++ << ": ";
      if (v.is<int>())
        std::cout << "int" << std::endl;
      else if (v.is<double>())
        std::cout << "double" << std::endl;
      else if (v.is<std::string>())
        std::cout << "string" << std::endl;
      else if (v.is<sol::table>())
        std::cout << "table" << std::endl;
      else
        std::cout << "unknown" << std::endl;
    }
  };

  // First register ValueType enum
  _state.new_enum<model::ValueType>("ValueType",
                                    {{"NUMBER", model::ValueType::NUMBER},
                                     {"TEXT", model::ValueType::TEXT},
                                     {"VOID", model::ValueType::VOID}});

  // Then register Value class
  _state.new_usertype<model::Value>(
      "Value",
      // Constructors
      sol::call_constructor,
      sol::constructors<model::Value(model::ValueType),
                        model::Value(const model::Value&),
                        model::Value(model::Value&&)>(),

      // Methods
      "set",
      sol::overload(&model::Value::set<int>, &model::Value::set<std::string>),
      "type", &model::Value::type,

      // Conversion operators
      "tonumber", [](const model::Value& v) { return int(v); }, "tostring",
      [](const model::Value& v) { return std::string(v); });

  _state.new_usertype<model::Character>(
      "Character", "pos", sol::protect(&model::Character::pos), "set_pos",
      sol::protect(&model::Character::set_pos), "set_rotation",
      sol::protect(&model::Character::set_rotation), "rotation",
      sol::protect(&model::Character::rotation));

  // Register ScopeTable
  _state.new_usertype<ScopeTable>(
      "ScopeTable", "add_variable", &ScopeTable::add_variable,
      "add_variable_global", &ScopeTable::add_variable_global,
      "lookup_variable", &ScopeTable::lookup_variable);

  // Define a sleep method
  _state.new_usertype<LuaGameEvent>(
      "ScriptEvent", sol::constructors<LuaGameEvent(std::string, sol::table)>(),
      "event_name", &LuaGameEvent::event_name, "data", &LuaGameEvent::data,
      sol::base_classes, sol::bases<common::EventBase>());
  _state["wait_for_event"] = [](sol::this_state s,
                                const std::string& event_name,
                                int timeout_ms = -1) {
    sol::state_view lua(s);
    ThreadContext& ctx =
        *lua.registry()["thread_context"].get<ThreadContext*>();

    auto event_name_ptr = std::make_shared<std::string>(event_name);
    auto token = common::EventDispatcher::instance().subscribe<LuaGameEvent>(
        [&ctx, event_name_ptr](auto event) {
          if (event->event_name == *event_name_ptr) {
            std::unique_lock lock(ctx.cv_mutex);
            ctx.event_received = true;
            ctx.event_data = event->data;
            ctx.cv.notify_one();
          }
        });

    logic_logger().info("Waiting for signal: {}", event_name);
    std::unique_lock lock(ctx.cv_mutex);

    bool received;
    if (timeout_ms == -1)
      ctx.cv.wait(lock, [&ctx] { return ctx.event_received; }), received = true;
    else
      received = ctx.cv.wait_for(lock, std::chrono::milliseconds(timeout_ms),
                                 [&ctx] { return ctx.event_received; });
    logic_logger().info("Signal \"{}\" received", event_name);
    logic_logger().info("Token Cancelled");
    return received ? ctx.event_data : sol::nil;
  };

  _state["emit_event"] = [&](sol::this_state s, const std::string& event_name,
                             sol::optional<sol::table> data_opt) {
    logic_logger().info("Emitting Signal: {}", event_name);
    sol::state_view lua(s);
    sol::table data = data_opt.value_or(lua.create_table());
    common::EventDispatcher::instance().publish(
        std::make_shared<LuaGameEvent>(event_name, data));
  };
  _state["wait"] = [](sol::this_state s, int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
  };
}
}  // namespace logic::lua
