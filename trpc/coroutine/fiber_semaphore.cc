#include "trpc/coroutine/fiber_semaphore.h"

#include <cassert>
#include <cstddef>
#include <mutex>

namespace trpc {

FiberSemaphore::FiberSemaphore(std::ptrdiff_t initial_count)
    : count_{initial_count} {
  assert(initial_count >= 0);
}

void FiberSemaphore::Post() {
  std::scoped_lock _(lock_);
  ++count_;
  cv_.notify_one();
}

void FiberSemaphore::Post(std::ptrdiff_t post_count) {
  std::scoped_lock _(lock_);
  count_ += post_count;
  cv_.notify_all();
}

void FiberSemaphore::Wait() {
  std::unique_lock lk(lock_);
  cv_.wait(lk, [this] { return count_ > 0; });
  --count_;
}

bool FiberSemaphore::TryWait() {
  std::scoped_lock _(lock_);
  if (count_ > 0) {
    --count_;
    return true;
  }
  return false;
}

}  // namespace trpc
