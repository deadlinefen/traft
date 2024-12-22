#pragma once

#include <vector>

#include "traft/raft/configuration.h"
#include "traft/raft/identity.h"
#include "traft/utils/define.h"

namespace traft::raft {

struct VoterPositionHint {
  std::int64_t peer_version{kInvalidIndex};
  std::int64_t old_peer_version{kInvalidIndex};
  int voter_offset{kInvalidIndex};
  int old_voter_offset{kInvalidIndex};
};

class Ticket {
 public:
  Ticket(const PeerId &peer) : peer_{peer}, voted_{false} {};

  bool operator==(const PeerId &peer) const { return peer_ == peer; }
  auto operator<=>(const PeerId &peer) const { return peer_ <=> peer; }

  bool MarkVoted() { return !voted_ && (voted_ = true); }

 private:
  PeerId peer_{};
  bool voted_{false};
};

class Ballot {
 public:
  Ballot(const Configuration &conf, const Configuration &old_conf);
  Ballot(Ballot &&) = default;
  Ballot &operator=(Ballot &&) = default;

  void GrantWithUpdateHint(const PeerId &peer, VoterPositionHint &hint);
  bool IsGranted() const { return quorum_ <= 0 && old_quorum_ <= 0; }

 private:
  auto GetVoterTicketWithUpdateHint_(const PeerId &peer, VoterPositionHint &hint);
  auto GetOldVoterTicketWithUpdateHint_(const PeerId &peer, VoterPositionHint &hint);

  std::vector<Ticket> voters_{};
  std::vector<Ticket> old_voters_{};
  std::int64_t conf_version_{kInvalidIndex};
  std::int64_t old_conf_version_{kInvalidIndex};
  int quorum_{0};
  int old_quorum_{0};
};

}  // namespace traft::raft
