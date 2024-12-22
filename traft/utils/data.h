#pragma once

#include <cstddef>

namespace traft::utils {

class Data {
 public:
  Data() = default;
  Data(void *data, size_t size) : data_(data), size_(size) {}
  Data(Data &&) = default;
  Data &operator=(Data &&) = default;

 private:
  void *data_{nullptr};
  size_t size_{0};
};

}  // namespace traft::utils
