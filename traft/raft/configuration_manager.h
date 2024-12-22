#pragma once

#include <deque>

#include "traft/raft/configuration.h"
#include "traft/raft/log_entry.h"
#include "traft/utils/error_code.h"

namespace traft::raft {

class ConfigurationManager {
 public:
  ConfigurationManager() = default;

  ErrorCode Add(LogEntry entry);

 private:
  std::deque<LogEntry> conf_entries_{};
  ConfigurationCtx snapshot_{};
};

}  // namespace traft::raft
