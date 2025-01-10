#include "traft/concurrentqueue/concurrentqueue.h"

#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <ranges>
#include <span>
#include <vector>

#include "gflags/gflags.h"
#include "gtest/gtest.h"

#include "trpc/common/config/trpc_config.h"
#include "trpc/common/runtime_manager.h"
#include "trpc/coroutine/fiber.h"

DEFINE_string(config, "fiber.yaml", "");

namespace traft::testing {

class ConcurrentQueueTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

constexpr std::size_t kFiberNum = 50;
constexpr std::size_t kTaskNum = 1000000;
constexpr std::size_t kDequeueBulk = kFiberNum;
constexpr std::size_t kTimeoutMs = 500;

constexpr auto fiber_range = std::views::iota(0UL, kFiberNum);
constexpr auto task_range = std::views::iota(0UL, kTaskNum);

void ParseClientConfig(int argc, char *argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::CommandLineFlagInfo info;
  if (GetCommandLineFlagInfo("config", &info) && info.is_default) {
    std::cerr << "start client with config, for example: " << argv[0]
              << " --config=/client/config/filepath" << std::endl;
    exit(-1);
  }

  std::cout << "FLAGS_config:" << FLAGS_config << std::endl;

  // 使用fiber client需要配置文件，且fiber环境配置正确(coroutine enable:
  // true,且有一个fiber的threadmodel配置)
  int ret = ::trpc::TrpcConfig::GetInstance()->Init(FLAGS_config);
  if (ret != 0) {
    std::cerr << "load config failed." << std::endl;
    exit(-1);
  }
}

TEST(ConcurrentQueueTest, NWriteOneRead) {
  ::trpc::RunInTrpcRuntime([]() {
    traft::ConcurrentQueue<std::size_t> queue;
    std::size_t counter = 0;

    std::vector<trpc::Fiber> fibers;
    fibers.reserve(kFiberNum + 1);

    // begin consumer fiber
    fibers.emplace_back([&] {
      std::array<std::size_t, kFiberNum> container;
      while (true) {
        std::size_t num = queue.WaitDequeueBulkTimed(
            container.begin(), kDequeueBulk, std::chrono::milliseconds{kTimeoutMs});
        if (num == 0) {
          break;
        }
        counter += num;
      }
      TRPC_FMT_INFO("consumer fiber done, counter: {}", counter);
    });

    // begin producer fiber
    for (auto fid : fiber_range) {
      fibers.emplace_back([&, fid] {
        for (auto i : task_range) {
          queue.Enqueue(std::size_t{i});
        }
        TRPC_FMT_INFO("producer fiber:{} done", fid);
      });
    }

    for (auto &f : fibers) {
      f.Join();
    }

    EXPECT_EQ(kFiberNum * kTaskNum, counter);

    return 0;
  });
}

TEST(ConcurrentQueueTest, NWriteToSerial) {
  ::trpc::RunInTrpcRuntime([]() {
    traft::ConcurrentQueue<std::size_t> input;
    traft::ConcurrentQueue<std::size_t> serial;

    std::vector<trpc::Fiber> fibers;
    fibers.reserve(kFiberNum + 2);

    // begin input consumer fiber
    fibers.emplace_back([&] {
      std::array<std::size_t, kFiberNum> container;
      std::size_t counter{0};

      while (true) {
        std::size_t num = input.WaitDequeueBulkTimed(
            container.begin(), kDequeueBulk, std::chrono::milliseconds{kTimeoutMs});
        if (num == 0) {
          break;
        }

        std::span<std::size_t> view{container.begin(), num};
        for (auto _ : view) {
          serial.Enqueue(std::size_t{++counter});
        }
      }

      EXPECT_EQ(kFiberNum * kTaskNum, counter);
      TRPC_FMT_INFO("input consumer fiber:{} done", counter);
    });

    // begin serial consumer fiber
    fibers.emplace_back([&] {
      std::array<std::size_t, kFiberNum> container;
      std::size_t last_index{0};
      std::size_t counter{0};

      while (true) {
        std::size_t num = serial.WaitDequeueBulkTimed(
            container.begin(), kDequeueBulk, std::chrono::milliseconds{kTimeoutMs});
        if (num == 0) {
          break;
        }

        std::span<std::size_t> view{container.begin(), num};
        for (auto index : view) {
          EXPECT_EQ(last_index + 1, index);
          last_index = index;
          ++counter;
        }
      }

      EXPECT_EQ(kFiberNum * kTaskNum, counter);
      TRPC_FMT_INFO("serial consumer fiber:{} done", last_index);
    });

    // begin input producer fiber
    for (auto fid : fiber_range) {
      fibers.emplace_back([&, fid] {
        for (auto i : task_range) {
          input.Enqueue(std::size_t{i});
        }
        TRPC_FMT_INFO("producer fiber:{} done", fid);
      });
    }

    for (auto &f : fibers) {
      f.Join();
    }

    return 0;
  });
}

}  // namespace traft::testing

int main(int argc, char *argv[]) {
  traft::testing::ParseClientConfig(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
