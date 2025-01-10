#pragma once

#include "spdlog/spdlog.h"

namespace traft {

extern void InitializeLogger();

#define TRAFT_LOG_TRACE(msg, ...) \
  SPDLOG_TRACE("[{}:{}] " msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define TRAFT_LOG_DEBUG(msg, ...) \
  SPDLOG_DEBUG("[{}:{}] " msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define TRAFT_LOG_INFO(msg, ...) \
  SPDLOG_INFO("[{}:{}] " msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define TRAFT_LOG_WARN(msg, ...) \
  SPDLOG_WARN("[{}:{}] " msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define TRAFT_LOG_ERROR(msg, ...) \
  SPDLOG_ERROR("[{}:{}] " msg, __FILE__, __LINE__, ##__VA_ARGS__)

}  // namespace traft
