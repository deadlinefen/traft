#pragma once

#include "proto/identity.pb.h"
#include "traft/utils/endpoint.h"

namespace traft::raft {

using GroupId = std::string;

enum class ReplicaType : int {
  DEFAULT,
  LEARNER,
  WITNESS,

  UNKNOWN = 999,
  ANY,
};

const char *ReplicaTypeToString(ReplicaType type);

class PeerId {
 public:
  PeerId() = default;
  PeerId(const utils::EndPoint &endpoint, int index, ReplicaType type)
      : endpoint_{endpoint}, index_{index}, type_{type} {}
  PeerId(const PBPeerId &pb)
      : endpoint_{pb.ip(), pb.port()},
        index_(pb.index()),
        type_(static_cast<ReplicaType>(pb.type())) {}

  PeerId &operator=(const PeerId &) = default;

  PeerId CopyWithoutReplicaType() const {
    return PeerId{endpoint_, index_, ReplicaType::ANY};
  }

  auto operator<=>(const PeerId &rhs) const {
    if (index_ != rhs.index_) {
      return index_ <=> rhs.index_;
    }
    return endpoint_ <=> rhs.endpoint_;
  }

  bool operator==(const PeerId &rhs) const {
    return index_ == rhs.index_ && endpoint_ == rhs.endpoint_;
  }

  bool operator!=(const PeerId &rhs) const { return !(*this == rhs); }

  bool IsEmpty() const { return endpoint_.IsAny() && index_ == 0; }

  bool IsLearnerReplica() const { return type_ == ReplicaType::LEARNER; }
  bool IsWitnessReplica() const { return type_ == ReplicaType::WITNESS; }
  bool IsDefaultReplica() const { return type_ == ReplicaType::DEFAULT; }

  std::string ToString() const;
  std::string Describe() const;

  std::string Serialize() const { return ConsturctPB_().SerializeAsString(); }

 private:
  PBPeerId ConsturctPB_() const;

  utils::EndPoint endpoint_{};
  int index_{0};
  ReplicaType type_{ReplicaType::DEFAULT};
};

class NodeId {
 public:
  NodeId() = default;
  NodeId(GroupId group_id, const PeerId &peer_id)
      : group_id_{std::move(group_id)}, peer_id_{peer_id} {}

  std::string ToString() const;
  std::string Describe() const;

 private:
  GroupId group_id_{};
  PeerId peer_id_{};
};

}  // namespace traft::raft
