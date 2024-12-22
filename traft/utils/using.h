#pragma once

#include <shared_mutex>

#include "spdlog/spdlog.h"

namespace traft {

using Mutex = std::mutex;
using SharedMutex = std::shared_mutex;

#define TRAFT_LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define TRAFT_LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define TRAFT_LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define TRAFT_LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define TRAFT_LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)

}  // namespace traft
