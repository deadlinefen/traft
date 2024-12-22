#pragma once

namespace traft {

enum class ErrorCode : int {
  OK = 0,

  // node layer
  NODE_APPEND_LOG_FAILED = 10000,
  NODE_UNEXPECTED_TERM = 10001,

  // log layer
  LOG_INDEX_INVALID = 20000,
};

}
