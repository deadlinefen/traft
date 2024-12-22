#include "traft/raft/ballot.h"

#include <algorithm>

namespace traft::raft {

Ballot::Ballot(const Configuration &conf, const Configuration &old_conf)
    : conf_version_{conf.GetVersion()}, old_conf_version_{old_conf.GetVersion()} {
  voters_.reserve(conf.VoterSize());
  const auto &voters = conf.GetVoters();
  for (auto &peer : conf.GetVoters()) {
    voters_.emplace_back(peer);
  }
  quorum_ = voters_.size() / 2 + 1;

  if (old_conf.VoterSize() < 1) {
    return;
  }

  old_voters_.reserve(old_conf.VoterSize());
  for (auto &peer : old_conf.GetVoters()) {
    old_voters_.emplace_back(peer);
  }
  old_quorum_ = old_voters_.size() / 2 + 1;
}

auto Ballot::GetOldVoterTicketWithUpdateHint_(const PeerId &peer,
                                              VoterPositionHint &hint) {
  if (old_conf_version_ == hint.old_peer_version) {
    return old_voters_.begin() + hint.old_voter_offset;
  } else if (old_conf_version_ == hint.peer_version) {
    hint.old_peer_version = old_conf_version_;
    hint.old_voter_offset = hint.voter_offset;
    return old_voters_.begin() + hint.voter_offset;
  }

  auto iter = std::find(old_voters_.begin(), old_voters_.end(), peer);
  if (iter != old_voters_.end()) {
    hint.old_peer_version = old_conf_version_;
    hint.old_voter_offset = iter - old_voters_.begin();
  }
  return iter;
}

auto Ballot::GetVoterTicketWithUpdateHint_(const PeerId &peer, VoterPositionHint &hint) {
  if (conf_version_ == hint.peer_version) {
    return voters_.begin() + hint.voter_offset;
  }

  auto iter = std::find(voters_.begin(), voters_.end(), peer);
  if (iter != voters_.end()) {
    hint.peer_version = conf_version_;
    hint.voter_offset = iter - voters_.begin();
  }

  return iter;
}

void Ballot::GrantWithUpdateHint(const PeerId &peer, VoterPositionHint &hint) {
  if (old_voters_.size() > 0) {
    auto ticket_iter = GetOldVoterTicketWithUpdateHint_(peer, hint);
    if (ticket_iter != old_voters_.end() && (*ticket_iter).MarkVoted()) {
      --old_quorum_;
    }
  }

  auto ticket_iter = GetVoterTicketWithUpdateHint_(peer, hint);
  if (ticket_iter != voters_.end() && (*ticket_iter).MarkVoted()) {
    --quorum_;
  }
}

}  // namespace traft::raft
