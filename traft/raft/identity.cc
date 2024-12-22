#include "traft/raft/identity.h"

#include <cstdio>
#include "proto/identity.pb.h"

namespace traft::raft {

const char *ReplicaTypeToString(ReplicaType type) {
  switch (type) {
    case ReplicaType::DEFAULT:
      return "dafault";
    case ReplicaType::LEARNER:
      return "learner";
    case ReplicaType::WITNESS:
      return "witness";
    case ReplicaType::ANY:
      return "any";
    default:
      return "unknown";
  }
}

std::string PeerId::Describe() const {
  return fmt::format("PeerId{{{}, index:{}, replica_type:{}}}", endpoint_.Describe(),
                     index_, ReplicaTypeToString(type_));
}

std::string PeerId::ToString() const {
  return fmt::format("{}:{}:{}", endpoint_.ToString(), index_,
                     ReplicaTypeToString(type_));
}

PBPeerId PeerId::ConsturctPB_() const {
  PBPeerId pb;
  pb.set_ip(endpoint_.Ip());
  pb.set_port(endpoint_.Port());
  pb.set_index(index_);
  pb.set_type(static_cast<int>(type_));

  return pb;
}

std::string NodeId::Describe() const {
  return fmt::format("NodeId{{group_id:{}, {}}}", group_id_, peer_id_.Describe());
}

std::string NodeId::ToString() const {
  return fmt::format("{}:{}", group_id_, peer_id_.ToString());
}

}  // namespace traft::raft
