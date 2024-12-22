#include "traft/raft/configuration.h"

#include "traft/raft/identity.h"

namespace traft::raft {

bool Configuration::AddVoter(PeerId peer_id) {
  if (ContainsLearner(peer_id)) {
    return false;
  }
  return voters_.insert(std::move(peer_id)).second;
}

bool Configuration::AddLearner(PeerId peer_id) {
  if (ContainsVoter(peer_id)) {
    return false;
  }
  return learners_.insert(std::move(peer_id)).second;
}

}  // namespace traft::raft
