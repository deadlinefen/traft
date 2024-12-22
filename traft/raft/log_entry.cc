#include "traft/raft/log_entry.h"

#include "fmt/format.h"

namespace traft::raft {

std::string LogId::ToString() const {
  return fmt::format("LogId{{index:{}, term:{}}}", term_, index_);
}

}  // namespace traft::raft
