#include "traft/raft/node.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <variant>

#include "traft/raft/log_entry.h"
#include "traft/utils/define.h"
#include "traft/utils/error_code.h"
#include "traft/utils/using.h"

namespace traft::raft {

// TODO: make it modifiable
constexpr int kRaftApplyBatch = 32;
// TODO: make it modifiable
constexpr std::int64_t kDequeueTimeoutMs = 1000;

ErrorCode RaftNode::Apply(Task &task) {
  if (!apply_queue_.Enqueue(UserLog{task})) {
    TRAFT_LOG_ERROR("node {} enqueue task failed", id_.ToString());
    return ErrorCode::NODE_APPEND_LOG_FAILED;
  }

  return ErrorCode::OK;
}

void RaftNode::ExecuteApplyUserLog_() {
  std::array<UserLog, kRaftApplyBatch> logs;

  while (running_) {
    size_t size = apply_queue_.WaitDequeueBulkTimed(logs.begin(), kRaftApplyBatch,
                                                    kDequeueTimeoutMs);
    if (size == 0) {
      continue;
    }

    DoApply_({logs.begin(), size});
  }
}

void RaftNode::DoApply_(std::span<UserLog> applying_logs) {
  std::unique_lock<Mutex> lck(mutex_);
  // TODO: add leader check

  for (auto &user_log : applying_logs) {
    if (user_log.expected_term_ != kInvalidIndex &&
        user_log.expected_term_ != current_term_) {
      TRAFT_LOG_WARN(
          "node {} can't apply taks whose expected_term={} doesn't match current_term={}",
          id_.ToString(), user_log.expected_term_, current_term_);

      continue;
    }

    std::visit(log_visitor_, user_log.GetEntry());
    user_log.log_.SetTerm(current_term_);
    // TODO: ballot_box append
    ballot_box_.LaunchBallot(conf_ctx_.GetConfiguration(),
                             conf_ctx_.GetOldConfiguration());
  }

  // TODO: log_manager append
  // TODO: log_manager check_and_set_configuration
}

}  // namespace traft::raft
