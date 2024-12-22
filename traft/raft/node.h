#pragma once

#include <cassert>
#include <memory>
#include <span>

#include "traft/concurrentqueue/concurrentqueue.h"
#include "traft/raft.h"
#include "traft/raft/ballot_box.h"
#include "traft/raft/configuration_manager.h"
#include "traft/raft/fsm_caller.h"
#include "traft/raft/log_entry.h"
#include "traft/utils/using.h"

namespace traft::raft {

class RaftNode;

class UserLog {
 public:
  UserLog() = default;
  UserLog(Task &task)
      : log_{std::move(task.data_)},
        expected_term_{task.expected_term_} {}
  UserLog(UserLog &&) = default;
  UserLog &operator=(UserLog &&) = default;

  Entry &GetEntry() { return log_.GetEntry(); }

 private:
  friend class RaftNode;

  LogEntry log_;
  int64_t expected_term_{kInvalidIndex};
};

class RaftNode : public std::enable_shared_from_this<RaftNode> {
 public:
  RaftNode() = default;

  class LogVisitor {
   public:
    LogVisitor(bool &running) : running_{running} {}

    void operator()(const QuitEntry &entry) { running_ = false; }
    void operator()(DataEntry &entry) {};
    void operator()(const ConfigurationEntry &entry) {
      assert(false && "User log should not be ConfigurationEntry");
    }
    void operator()(const NoOpEntry &entry) {
      assert(false && "User log should not be NoOpEntry");
    }

   private:
    bool &running_;
  };

  ErrorCode Apply(Task &task);

 private:
  friend class LogVisitor;

  void ExecuteApplyUserLog_();
  void DoApply_(std::span<UserLog> applying_logs);

  NodeId id_{};
  LogVisitor log_visitor_{running_};
  Mutex mutex_;
  FSMCaller fsm_caller_{};

  ConcurrentQueue<UserLog> apply_queue_;
  ConfigurationCtx conf_ctx_;
  ConfigurationManager config_manger_;
  std::int64_t current_term_{0};
  BallotBox ballot_box_{fsm_caller_};
  bool running_{false};
};

}  // namespace traft::raft
