#include "traft/raft/configuration_manager.h"
#include "traft/raft/log_entry.h"
#include "traft/utils/error_code.h"
#include "traft/utils/using.h"

namespace traft::raft {

ErrorCode ConfigurationManager::Add(LogEntry entry) {
  // check if the last entry is greater than the new entry
  if (!conf_entries_.empty()) {
    LogEntry &last_entry = conf_entries_.back();
    if (last_entry.GetIndex() >= entry.GetIndex()) {
      TRAFT_LOG_ERROR(
          "Did you forget to call truncate_suffix before the last log index goes back? "
          "last_entry.index={}, add_entry.index={}",
          last_entry.GetIndex(), entry.GetIndex());
      return ErrorCode::LOG_INDEX_INVALID;
    }
  }

  conf_entries_.emplace_back(std::move(entry));
  return ErrorCode::OK;
}

}  // namespace traft::raft
