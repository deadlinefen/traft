#include <array>
#include <cstddef>
#include <ranges>
#include <vector>

#include "gtest/gtest.h"
#include "testing/common/fiber_runtime.h"
#include "traft/concurrentqueue/concurrentqueue.h"
#include "trpc/coroutine/fiber.h"

namespace traft::testing {

constexpr std::size_t kFiberNum = 10;
constexpr std::size_t kTaskNum = 1000;
constexpr std::size_t kDequeueBulk = kFiberNum;
constexpr std::size_t kTimeoutMs = 1000;

constexpr auto fiber_range = std::views::iota(0UL, kFiberNum);
constexpr auto task_range = std::views::iota(0UL, kTaskNum);

TEST(ConcurrentQueue, NFiberWrite1FiberRead) {
  FiberRuntimeGuard _;

  traft::ConcurrentQueue<std::size_t> queue;
  std::size_t counter = 0;

  std::vector<trpc::Fiber> fibers;
  fibers.reserve(kFiberNum + 1);

  // begin consumer fiber
  fibers.emplace_back([&] {
    std::array<std::size_t, kFiberNum> container;
    while (true) {
      std::size_t num =
          queue.WaitDequeueBulkTimed(container.begin(), kDequeueBulk, kTimeoutMs);
      if (num == 0) {
        break;
      }
      counter += num;
    }
  });

  // begin producer fiber
  for (auto _ : fiber_range) {
    fibers.emplace_back([&] {
      for (auto i : task_range) {
        queue.Enqueue(std::size_t{i});
      }
    });
  }

  for (auto &f : fibers) {
    f.Join();
  }

  ASSERT_EQ(kFiberNum * kTaskNum, counter);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace traft::test
