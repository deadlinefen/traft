#pragma once

#include <cstddef>

#include "trpc/coroutine/fiber_condition_variable.h"
#include "trpc/coroutine/fiber_mutex.h"

namespace trpc {

class FiberSemaphore {
 public:
  explicit FiberSemaphore(std::ptrdiff_t initial_count);

  void Post();
  void Post(std::ptrdiff_t post_count);

  void Wait();

  bool TryWait();

  template <class Rep, class Period>
  bool TimedWait(const std::chrono::duration<Rep, Period> &timeout) {
    std::unique_lock lk(lock_);
    return cv_.wait_for(lk, timeout, [this] { return count_ > 0; });
  }

 private:
  mutable FiberMutex lock_;
  mutable FiberConditionVariable cv_;
  std::ptrdiff_t count_;
};

}  // namespace trpc
