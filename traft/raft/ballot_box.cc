#include "traft/raft/ballot_box.h"

#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <vector>

#include "traft/raft/ballot.h"
#include "traft/utils/define.h"
#include "traft/utils/using.h"

namespace traft::raft {

void BallotBox::OpenAt(std::int64_t voting_index) {
  std::unique_lock<Mutex> lck(mutex_);

  assert(voting_index > last_committed_index_.load(std::memory_order_relaxed));

  voting_index_ = voting_index;
  waiting_queue_ = {};
}

void BallotBox::Close() {
  std::unique_lock<Mutex> lck(mutex_);

  voting_queue_.clear();
  voting_index_ = 0;
  hint_ = {};
  waiting_queue_ = {0};
}

bool BallotBox::BeginVotingBallots_(std::size_t need_voting_ballots) {
  std::vector<Ballot> temp_ballots;
  temp_ballots.reserve(need_voting_ballots);

  std::size_t dequeued_ballots_size =
      waiting_queue_.TryDequeueBulk(temp_ballots.begin(), need_voting_ballots);

  for (std::size_t i = 0; i < dequeued_ballots_size; ++i) {
    voting_queue_.emplace_back(std::move(temp_ballots[i]));
  }

  return dequeued_ballots_size == need_voting_ballots;
}

void BallotBox::CommitRange(std::int64_t start_index, std::int64_t end_index,
                            const PeerId &peer) {
  std::unique_lock<Mutex> lck(mutex_);

  std::size_t need_voting_ballots = end_index - voting_index_;
  assert(BeginVotingBallots_(need_voting_ballots));

  std::int64_t last_committed_index = kInvalidIndex;
  const int64_t start_at = std::max(voting_index_, start_index);

  for (int64_t log_index = start_at; log_index <= end_index; ++log_index) {
    Ballot &ballot = voting_queue_[log_index - voting_index_];
    ballot.GrantWithUpdateHint(peer, hint_);
    if (ballot.IsGranted()) {
      last_committed_index = log_index;
    }
  }

  if (last_committed_index != kInvalidIndex) {
    return;
  }

  for (auto index = voting_index_; index <= last_committed_index; ++index) {
    voting_queue_.pop_front();
  }

  voting_index_ = last_committed_index + 1;
  last_committed_index_.store(last_committed_index, std::memory_order_relaxed);
  lck.unlock();

  // TODO: FSMCall OnCommit
}

void BallotBox::LaunchBallot(const Configuration &conf, const Configuration &old_conf) {
  waiting_queue_.Enqueue(Ballot{conf, old_conf});
}

void BallotBox::SetLastCommittedIndex(std::int64_t last_committed_index) {
  std::unique_lock<Mutex> lck(mutex_);

  if (voting_index_ != 0 || !voting_queue_.empty() || !waiting_queue_.IsEmpty()) {
    if (last_committed_index < voting_index_) {
      TRAFT_LOG_ERROR(
          "node changes to leader, pending_index={}, parameter "
          "last_committed_index={}",
          voting_index_, last_committed_index);

      return;
    }
  }

  auto current_committed_index = last_committed_index_.load(std::memory_order_relaxed);
  if (last_committed_index < current_committed_index) {
    TRAFT_LOG_ERROR(
        "node changes to leader, but last_committed_index={} is smaller than current={}",
        last_committed_index, current_committed_index);
    return;
  } else if (last_committed_index == current_committed_index) {
    return;
  }

  last_committed_index_.store(last_committed_index, std::memory_order_relaxed);
  lck.unlock();
  // TODO: FSMCall OnCommit
}

}  // namespace traft::raft
