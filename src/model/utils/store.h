#pragma once
#include <map>
#include <memory>
#include "ID_manager.h"

namespace model {
template <typename Object>
class Store {
public:
  Store(std::unique_ptr<IDManager>);

  template <typename... T>
  std::shared_ptr<Object> create_entity(T&&... args);
  void remove_entity(const IDManager::IDType&);

  bool has_entity(const std::string& id) const;
  std::shared_ptr<Object> get_entity(const std::string& id) const;

private:
  std::unique_ptr<IDManager> _id_mgr;
  std::map<std::string, std::shared_ptr<Object>> _store;
};

// Implementation
template <typename Object>
Store<Object>::Store(std::unique_ptr<IDManager> mgr)
    : _id_mgr(std::move(mgr)) {}

template <typename Object>
bool Store<Object>::has_entity(const std::string& id) const {
  return _store.count(id);
}
template <typename Object>
std::shared_ptr<Object> Store<Object>::get_entity(const std::string& id) const {
  assert(has_entity(id));
  return _store.at(id);
}

template <typename Object>
template <typename... T>
std::shared_ptr<Object> Store<Object>::create_entity(T&&... args) {
  auto new_id = _id_mgr->generate_with_counter();
  _store[new_id] =
      std::make_shared<Object>(new_id, std::forward<decltype(args)>(args)...);
  return _store[new_id];
}
template <typename Object>
void Store<Object>::remove_entity(const IDManager::IDType& id) {
  assert(_store.count(id) && "Store doesn't have this ID: " + id);
  _store.erase(id);
}
}  // namespace model
