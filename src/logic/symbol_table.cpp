#include "symbol_table.h"
#include <shared_mutex>
#include "error/global_scope_cannot_be_popped.h"
#include "error/value_not_found.h"
#include "logic_logger.h"
namespace logic {

ScopeTable::ScopeTable(std::shared_ptr<ScopeTable> parent_scope)
    : _parent_scope(parent_scope) {}

void ScopeTable::add_variable(const std::string& label, model::Value value) {
  std::unique_lock lck(_table_mtx);

  // Check if variable already exists in current scope
  if (_table.find(label) != _table.end()) {
    _table[label] = value;
    logic_logger().warn("{} variable has been overwriteen", label);
  } else {
    _table.insert({label, value});
    logic_logger().warn("{} variable has been inserted", label);
  }
}

model::Value& ScopeTable::lookup_variable(const std::string& label) {
  std::shared_lock lck(_table_mtx);
  auto itr = _table.find(label);

  return itr->second;

  // Value is not found
  if (_parent_scope != nullptr) return _parent_scope->lookup_variable(label);
  throw error::ValueNotFound(label);
}
}  // namespace logic
