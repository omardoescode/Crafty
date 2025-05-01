#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <sol/sol.hpp>
#include "lua_impl/lua_state_manager.h"

namespace logic::lua {

/**
 * @brief thread information
 */
struct ThreadContext {
  std::thread thread;
  std::shared_ptr<LuaStateManager> mgr;

  std::mutex cv_mutex;
  std::condition_variable cv;
  bool event_received = false;
  sol::table event_data;
};

class ThreadPool {
public:
  /**
   * @brief a Type alias to a thread function
   */
  typedef std::function<void(std::shared_ptr<LuaStateManager>)> ThreadFunction;

  /**
   * @brief Constructor
   * @param size_t pool_size
   */
  ThreadPool(std::size_t pool_size);

  ~ThreadPool();

  /**
   * @brief Enqueue A Task to be executed by the threads
   */
  void enqueue(ThreadFunction);

private:
  std::condition_variable _cond;
  std::list<ThreadContext> _pool;
  std::mutex _pool_mtx;
  std::queue<ThreadFunction> _queue;
  std::mutex _queue_mtx;
  bool _stop;
};
}  // namespace logic::lua
