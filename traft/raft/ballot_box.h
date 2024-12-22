#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <deque>

#include "traft/concurrentqueue/concurrentqueue.h"
#include "traft/raft/ballot.h"
#include "traft/raft/configuration.h"
#include "traft/raft/identity.h"
#include "traft/utils/define.h"
#include "traft/utils/using.h"

namespace traft::raft {

class FSMCaller;

class BallotBox {
 public:
  BallotBox(FSMCaller &caller) : caller_{caller}, waiting_queue_{0} {}

  void OpenAt(std::int64_t voting_index);
  void Close();

  void CommitRange(std::int64_t start_index, std::int64_t end_index, const PeerId &peer);

  void LaunchBallot(const Configuration &conf, const Configuration &old_conf);

  void SetLastCommittedIndex(std::int64_t index);

  inline std::int64_t GetLastCommittedIndex() const {
    return last_committed_index_.load(std::memory_order_acquire);
  }

 private:
  bool BeginVotingBallots_(std::size_t need_voting_ballots);

  Mutex mutex_;

  FSMCaller &caller_;
  std::atomic<std::int64_t> last_committed_index_{kInvalidIndex};
  std::deque<Ballot> voting_queue_;
  std::int64_t voting_index_{0};
  VoterPositionHint hint_{};
  ConcurrentQueue<Ballot> waiting_queue_;
};

}  // namespace traft::raft
