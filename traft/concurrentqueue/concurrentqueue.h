#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>

#include "traft/concurrentqueue/blockingconcurrentqueue_impl.h"

namespace traft {

template <typename T>
concept TraftQueuableElement =
    std::is_constructible_v<T> && std::is_move_constructible_v<T>;

template <typename Queue, typename Elem>
class ConcurrentQueueInterface {
 public:
  ConcurrentQueueInterface() = default;
  ConcurrentQueueInterface(std::size_t capacity) : queue_{capacity} {}

  bool Enqueue(Elem &&e) { return queue_.Enqueue(std::forward<Elem>(e)); }

  template <typename It>
    requires std::same_as<Elem, typename std::iterator_traits<It>::value_type>
  std::size_t WaitDequeueBulkTimed(It it, std::size_t max_size,
                                   std::int64_t timeout) {
    return queue_.WaitDequeueBulkTimed(it, max_size, timeout);
  }

  template <typename It>
    requires std::same_as<Elem, typename std::iterator_traits<It>::value_type>
  std::size_t TryDequeueBulk(It it, std::size_t max_size) {
    return queue_.TryDequeueBulk(it, max_size);
  }

  std::size_t IsEmpty() const { return queue_.IsEmpty(); }

  std::size_t SizeApprox() const { return queue_.SizeApprox(); }

 private:
  Queue queue_;
};

template <typename Elem>
class ConcurrentQueueV1 {
 public:
  ConcurrentQueueV1() = default;
  ConcurrentQueueV1(std::size_t capacity) : queue_{capacity} {}

  bool Enqueue(Elem &&e) { return queue_.enqueue(std::forward<Elem>(e)); }

  template <typename It>
  std::size_t WaitDequeueBulkTimed(It it, std::size_t max_size,
                                   std::int64_t timeout_ms) {
    // step 1: try to dequeue from the queue without block
    std::size_t size = queue_.try_dequeue_bulk(it, max_size);
    if (size != 0) {
      return size;
    }

    // step 2: block and wait for the queue to be non-empty
    if (queue_.wait_dequeue_timed(*it, timeout_ms * 1000)) {
      return queue_.try_dequeue_bulk(++it, max_size - 1) + 1;
    }

    return 0;
  }

  template <typename It>
  std::size_t TryDequeueBulk(It it, std::size_t max_size) {
    return queue_.try_dequeue_bulk(it, max_size);
  }

  std::size_t IsEmpty() const { return queue_.size_approx() > 0; }

  std::size_t SizeApprox() const { return queue_.size_approx(); }

 private:
  moodycamel::BlockingConcurrentQueue<Elem> queue_;
};

template <typename Elem>
using ConcurrentQueue = ConcurrentQueueInterface<ConcurrentQueueV1<Elem>, Elem>;

}  // namespace traft
