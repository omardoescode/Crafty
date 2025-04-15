#include "lua_interpreter.h"
#include <format>
#include <memory>
#include <mutex>
#include <sol/make_reference.hpp>
#include <sol/raii.hpp>
#include <sol/table.hpp>
#include "block/input_slot_instance.h"
#include "block/value.h"
#include "block/value_type.h"
#include "events/event_dispatcher.h"
#include "logic_logger.h"
#include "model_events.h"
#include "project_manager.h"
#include "scope_table.h"

static auto& dispatcher = common::EventDispatcher::instance();
static auto& mgr = model::ProjectManager::instance();

namespace logic {
LuaInterpreter::LuaInterpreter()
    : _status(IDLE), _global_ctx(std::make_shared<ScopeTable>()) {
  initialize_state();
  initialize_usertypes();
  remove_script_tkn = dispatcher.subscribe<model::events::beforeScriptDeleted>(
      [this](auto evt) { _scripts.erase(evt->script); });
}

LuaInterpreter::~LuaInterpreter() {
  for (auto& thr : _threads) thr->thread.join();
}

void LuaInterpreter::initialize_state() {
  logic_logger().info("Initializing State");
  std::lock_guard lck(_lua_state_mtx);
  _lua_state.open_libraries(sol::lib::base, sol::lib::package);

  // Initialize Categories
  auto& mgr = model::ProjectManager::instance();
  auto categories = mgr.block_lib()->categories();
  std::string fmt = "lua/{}.lua";
  for (auto& category : categories) {
    std::string file_path = std::vformat(fmt, std::make_format_args(category));
    _lua_state.require_file(category, file_path);
    logic_logger().info("Lua Module {} initialized", file_path);
  }
}

void LuaInterpreter::initialize_usertypes() {
  // First register ValueType enum
  _lua_state.new_enum<model::ValueType>("ValueType",
                                        {{"NUMBER", model::ValueType::NUMBER},
                                         {"TEXT", model::ValueType::TEXT},
                                         {"VOID", model::ValueType::VOID}});

  // Then register Value class
  _lua_state.new_usertype<model::Value>(
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

  // Register ScopeTable (existing code)
  _lua_state.new_usertype<ScopeTable>(
      "ScopeTable", "add_variable", &ScopeTable::add_variable,
      "add_variable_global", &ScopeTable::add_variable_global,
      "lookup_variable", &ScopeTable::lookup_variable);
}

void LuaInterpreter::register_script(std::shared_ptr<model::Script> script) {
  std::lock_guard lck(_scripts_mtx);
  _scripts.insert(script);
}
void LuaInterpreter::execute() {
  _threads.emplace_back(std::make_unique<ThreadContext>());
  auto& ctx = _threads.back();

  ctx->thread = std::thread([&, this]() {
    std::lock_guard lck(_scripts_mtx);
    for (auto& script : _scripts) {
      execute(script, _global_ctx);
    }

    // clear scripts
    _scripts.clear();

    // Change state
    ctx->is_running = false;
  });
}

void LuaInterpreter::execute(std::shared_ptr<model::Script> script,
                             std::shared_ptr<ScopeTable> parent_table) {
  auto scope = std::make_shared<ScopeTable>(parent_table);
  static auto& instance_store = mgr.project()->instances_store();
  auto instances = script->blocks();
  for (auto instance_w : instances) {
    auto instance_id = instance_w.lock();
    if (!instance_id) continue;

    auto instance = instance_store.get_entity(instance_id);
    execute_block(instance, scope);
  }
}

model::Value LuaInterpreter::execute_block(
    std::shared_ptr<model::BlockInstance> instance,
    std::shared_ptr<ScopeTable> current_table) {
  // Handle inputs
  for (std::shared_ptr<model::InputSlotInstance> input : instance->inputs()) {
    if (input->has_block()) {
      // TODO: TEST THIS PART
      model::Value value = execute_input_slot(input, current_table);
      current_table->add_variable(input->def().label(), value);
    }
  }

  {
    std::unique_lock lck(_lua_state_mtx);

    // Create a new Lua environment for this execution
    sol::environment env(_lua_state, sol::create, _lua_state.globals());

    // Create the argument list
    sol::table block_ctx = _lua_state.create_table();
    block_ctx["scope"] = current_table.get();
    // Execute the script in this environment
    try {
      sol::protected_function func =
          _lua_state[instance->def()->category()][instance->def()->data_id()];
      auto result = func(block_ctx);

      if (!result.valid()) {
        sol::error err = result;
        logic_logger().error("Lua execution error: {}", err.what());
      }
    } catch (const sol::error& e) {
      logic_logger().error("Lua execution error: {}", e.what());
    }
  }

  return model::Value(model::ValueType::VOID);
}

Interpreter::Status LuaInterpreter::status() { return _status; }

model::Value LuaInterpreter::execute_input_slot(
    std::shared_ptr<model::InputSlotInstance> instance,
    std::shared_ptr<ScopeTable> current_table) {
  return model::Value(model::ValueType::VOID);
}
}  // namespace logic
