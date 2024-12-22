#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "traft/utils/define.h"

namespace traft::storage {

constexpr int kInvalidFd = -1;
constexpr int64_t kInvalidIndex = -1;

class alignas(kCacheLineByte) Segment {
 public:
  Segment(std::string path, const int64_t first_index, const int64_t last_index)
      : _path{std::move(path)}, _first_index{first_index}, _last_index{last_index} {}
  ~Segment();

  struct EntryHeader;

  int Open();
  int Close();

 private:
  std::string GetOpenPath_();
  uint32_t GetChecksum_(std::string_view sv);

  std::string _path;
  int64_t _first_index{kInvalidIndex};
  int64_t _last_index{kInvalidIndex};
  std::size_t _size{0};
  bool _is_open{false};
  int _fd{kInvalidFd};
  std::vector<std::pair<int64_t, int64_t>> _offset_and_term;
};

}  // namespace traft::storage
