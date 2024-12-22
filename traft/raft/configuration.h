#pragma once

#include <cstddef>
#include <set>

#include "traft/raft/identity.h"
#include "traft/raft/log_entry.h"

namespace traft::raft {

class Configuration {
 public:
  Configuration() = default;

  void Reset() {
    voters_.clear();
    learners_.clear();
  }

  bool operator==(const Configuration &rhs) const = default;

  bool IsEmpty() const { return voters_.empty() && learners_.empty(); }

  size_t VoterSize() const { return voters_.size(); }
  size_t LearnerSize() const { return learners_.size(); }

  const std::set<PeerId> &GetVoters() const { return voters_; }
  const std::set<PeerId> &GetLearners() const { return learners_; }

  std::int64_t GetVersion() const { return version_; }
  void SetVersion(std::int64_t version) { version_ = version; }

  bool ContainsVoter(const PeerId &peer_id) const {
    return voters_.find(peer_id) != voters_.end();
  }
  bool ContainsLearner(const PeerId &peer_id) const {
    return learners_.find(peer_id) != learners_.end();
  }

  bool AddVoter(PeerId peer_id);
  bool AddLearner(PeerId peer_id);

  bool RemoveVoter(const PeerId &peer_id) { return voters_.erase(peer_id); }
  bool RemoveLearner(const PeerId &peer_id) { return learners_.erase(peer_id); }

 private:
  std::int64_t version_{0};
  std::set<PeerId> voters_;
  std::set<PeerId> learners_;
};

class ConfigurationCtx {
 public:
  ConfigurationCtx() = default;
  ConfigurationCtx(ConfigurationEntry &entry) {
    // TODO: add constructor
  }

  bool IsStable() const { return old_conf_.IsEmpty(); }
  bool IsEmpty() const { return conf_.IsEmpty(); }
  bool ContainsVoter(const PeerId &peer) const {
    return conf_.ContainsVoter(peer) || old_conf_.ContainsVoter(peer);
  }

  bool ContainsLearner(const PeerId &peer) const {
    return conf_.ContainsLearner(peer) || old_conf_.ContainsLearner(peer);
  }

  const Configuration &GetConfiguration() const { return conf_; }
  const Configuration &GetOldConfiguration() const { return old_conf_; }

 private:
  Configuration conf_{};
  Configuration old_conf_{};
};

}  // namespace traft::raft
