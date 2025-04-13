#pragma once
#include <cassert>
#include <map>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <type_traits>
#include "id.h"
#include "id_generator.h"
#include "identity/identifiable.h"

namespace model {
template <typename Object>
class Store {
public:
  /**
   * @brief Constructor for store
   * @param id_generator An ID generator for entities of type `Object`
   */
  Store(std::unique_ptr<IDGenerator>);

  /**
   * @brief Create an entity of type `Object`
   * @param args The arguments passed to the constructor without ID
   * @return a shared point to the newly created Object
   *
   * @note This method inserts the `id` argument before `args`
   */
  template <typename... T>
  std::shared_ptr<Object> create_entity(T&&... args);

  /**
   * @brief Remove an entity from the store with the specified id
std::unique_ptr<IDGeneratorstd::unique_ptr<IDGenerator>>   * @param id The
entity's id to remove
   */
  void remove_entity(IDPtr id);

  /**
   * @brief Check for the existence of an entity with the specified id
   * @param id The id of the object to search for
   * @return true if an object with this id exists in the store
   */
  bool has_entity(IDPtr id) const;

  /**
   * @brief Retrieve the entity with the specified id
   * @param id The id of the entity to retrieve
   * @return A shared pointer to the entity if exists
   */
  std::shared_ptr<Object> get_entity(IDPtr id) const;

  /**
   * @brief Retrieve the entity with the specified id
   * @param id A weak pointer to the specified id
   * @return A shared pointer to the entity if exists
   */
  std::shared_ptr<Object> get_entity(IDWPtr id) const;

private:
  IDGeneratorPtr _id_mgr;
  std::map<IDPtr, std::shared_ptr<Object>> _store;
  mutable std::shared_mutex _store_mtx;
};

// Implementation
template <typename Object>
Store<Object>::Store(std::unique_ptr<IDGenerator> mgr)
    : _id_mgr(std::move(mgr)) {
  static_assert(std::is_base_of<Identifiable, Object>());
}

template <typename Object>
bool Store<Object>::has_entity(IDPtr id) const {
  std::shared_lock lck(_store_mtx);
  return _store.count(id);
}
template <typename Object>
std::shared_ptr<Object> Store<Object>::get_entity(IDPtr id) const {
  std::shared_lock lck(_store_mtx);
  assert(has_entity(id));
  return _store.at(id);
}

template <typename Object>
template <typename... T>
std::shared_ptr<Object> Store<Object>::create_entity(T&&... args) {
  std::unique_lock lck(_store_mtx);
  auto new_id = _id_mgr->generate_next();
  _store[new_id] =
      std::make_shared<Object>(new_id, std::forward<decltype(args)>(args)...);
  return _store[new_id];
}
template <typename Object>
void Store<Object>::remove_entity(IDPtr id) {
  std::unique_lock lck(_store_mtx);
  assert(_store.count(id) && "Store doesn't have this ID");
  _store.erase(id);
}

template <typename Object>
std::shared_ptr<Object> Store<Object>::get_entity(IDWPtr id_w) const {
  auto id = id_w.lock();
  if (!id) return nullptr;
  return get_entity(id);
}
}  // namespace model
