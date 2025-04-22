#include "thread_pool.h"
#include <mutex>
#include "lua_impl/lua_module_cache.h"

namespace logic::lua {
ThreadPool::ThreadPool(std::size_t pool_size) : _stop(false) {
  for (int i = 0; i < pool_size; i++) {
    std::unique_lock lck(_queue_mtx);
    _pool.emplace_back();
    auto& ctx = _pool.back();
    ctx.thread = std::thread([&ctx, this]() {
      auto& cache = LuaModuleCache::instance();
      ctx.mgr = std::make_shared<LuaStateManager>();
      ctx.mgr->initialize(cache);
      while (true) {
        ThreadFunction task;
        {
          std::unique_lock lck(_queue_mtx);
          _cond.wait(lck, [this] { return !_queue.empty() || _stop; });
          // Exit only in case there are no tasks left and we are told to stop
          if (_stop && _queue.empty()) {
            return;
          }
          task = std::move(_queue.front());
          _queue.pop();
        }
        task(ctx.mgr);
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock lck(_queue_mtx);
    _stop = true;
  }
  _cond.notify_all();
  for (auto& ctx : _pool) {
    ctx.thread.join();
  }
}

void ThreadPool::enqueue(ThreadPool::ThreadFunction task) {
  {
    std::unique_lock<std::mutex> lock(_queue_mtx);
    if (_stop) return;
    _queue.push(std::move(task));
  }
  _cond.notify_one();
}

}  // namespace logic::lua
