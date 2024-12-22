#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>

#include "concurrentqueue/blockingconcurrentqueue.h"
#include "liburing.h"
#include "liburing/io_uring.h"
#include "traft/storage/segment.h"

namespace traft::storage {

class IOManager;

class FlushThread {
 public:
  FlushThread(const size_t uring_depth, IOManager &io_manager,
              const io_uring_params &params)
      : uring_depth_{uring_depth}, io_manager_{io_manager}, params_{params} {
    if (!InitSucceed_()) {
      throw std::runtime_error("Failed to init io_uring");
    }
  }

  FlushThread(const size_t uring_depth, IOManager &io_manager)
      : uring_depth_{uring_depth}, io_manager_{io_manager} {
    if (!InitSucceed_()) {
      throw std::runtime_error("Failed to init io_uring");
    }
  }

  void operator()();

 private:
  bool InitSucceed_();

  std::atomic<bool> running_{false};
  std::mutex mutex_;
  std::condition_variable cv_;

  const size_t uring_depth_{};
  IOManager &io_manager_;
  io_uring ring_{};
  io_uring_params params_{};
};

using FlushQueue = moodycamel::BlockingConcurrentQueue<std::shared_ptr<Segment>>;

class IOManager {
 private:
  friend class FlushThread;

  FlushQueue flush_queue_;
};

}  // namespace traft::storage
