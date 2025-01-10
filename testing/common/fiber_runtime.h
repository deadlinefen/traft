#pragma once

#include "trpc/runtime/fiber_runtime.h"

namespace traft::testing {

class FiberRuntimeGuard {
 public:
  FiberRuntimeGuard() { trpc::fiber::StartRuntime(); }
  ~FiberRuntimeGuard() { trpc::fiber::TerminateRuntime(); }
};

}  // namespace traft::test
