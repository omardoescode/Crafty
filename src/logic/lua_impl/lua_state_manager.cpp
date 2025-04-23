#include "lua_impl/lua_state_manager.h"
#include "character.h"
#include "lua_impl/lua_module_cache.h"
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

  _state["script_start"] = []() { std::cout << "HELLO??" << std::endl; };

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
}
}  // namespace logic::lua
