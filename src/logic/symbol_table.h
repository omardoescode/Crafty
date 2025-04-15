#pragma once

#include <list>
#include <map>
#include <memory>
#include <shared_mutex>
#include <string>
#include "block/value.h"

namespace logic {
/**
 * @brief a data structure to organize looking up variable values
 * during runtime execution
 *
 * A scope is a new environment that has all the past variable and we can
 * temporarly add values
 *
 * A scope table has a parent scope unless for the global scope, having a
 * `parent_scope` of nullptr
 */
class ScopeTable {
public:
  /**
   * @breif Constructor constructor for scope table
   * @param parent_scope The parent scope of this scope. Will be lookuped up
   * into in case it a label doesn't exist in the current scope
   */
  ScopeTable(std::shared_ptr<ScopeTable> parent_scope = nullptr);
  /**
   * @brief Add a new variable to the current scope
   * @note If a variable with this label in current scope, it will override it
   * @param label the variable name. Used to look up
   * @param value the initial value of the variable
   */
  void add_variable(const std::string& label, model::Value value);

  /**
   * @brief Lookup a variable with a specific lable
   * @param label The variable name
   * @return A reference to the value of the variable
   * @throw ValueNotFound in case it doesn't exist
   */
  model::Value& lookup_variable(const std::string& label);

private:
  std::shared_ptr<ScopeTable> _parent_scope;
  std::map<std::string, model::Value> _table;
  mutable std::shared_mutex _table_mtx;  // assuming a heavy read symbol table.
                                         // TODO: Is this true tho?
};
}  // namespace logic
