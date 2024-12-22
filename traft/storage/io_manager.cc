#include "traft/storage/io_manager.h"

#include <atomic>
#include <vector>

#include "traft/utils/using.h"

namespace traft::storage {

bool FlushThread::InitSucceed_() {
  if (io_uring_queue_init_params(uring_depth_, &ring_, &params_) < 0) {
    TRAFT_LOG_ERROR("flush thread init io_uring failed");
    return false;
  }

  return true;
}

void FlushThread::operator()() {
  running_.store(true);

  std::vector<std::shared_ptr<Segment>> segments(uring_depth_);
  auto timeout = std::chrono::milliseconds(1000);
}

}  // namespace traft::storage
