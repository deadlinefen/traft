#include "traft/utils/logger.h"

#include "spdlog/common.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace traft {

static bool log_initialized = false;
static const std::string kTraftLoggerName = "traft";

extern void InitializeLogger() {
  if (log_initialized) {
    return;
  }

  auto traft_logger = spdlog::stdout_color_mt(kTraftLoggerName);
  // TODO: set level
  traft_logger->set_level(spdlog::level::debug);
  traft_logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
  spdlog::set_default_logger(traft_logger);

  log_initialized = true;
}

}  // namespace traft
