#pragma once

#include <utility>

#include "traft/utils/data.h"
#include "traft/utils/define.h"

namespace traft {

namespace raft {
class UserLog;
}

class Task {
 public:
  Task(utils::Data data) : data_(std::move(data)) {}
  Task(utils::Data data, int64_t expected_term)
      : data_(std::move(data)), expected_term_(expected_term) {}

  Task(const Task &) = delete;

 private:
  friend class raft::UserLog;

  utils::Data data_;
  int64_t expected_term_{kInvalidIndex};
};

}  // namespace traft
