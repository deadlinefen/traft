#pragma once

#include <shared_mutex>


namespace traft {

using Mutex = std::mutex;
using SharedMutex = std::shared_mutex;

}  // namespace traft
