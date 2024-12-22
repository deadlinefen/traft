#include "traft/storage/segment.h"

#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <string_view>

#include "fmt/format.h"
#include "traft/utils/using.h"
#include "xxHash/xxhash.h"

namespace traft::storage {

Segment::~Segment() {
  if (_is_open && _fd > kInvalidFd) {
    ::close(_fd);
  }
}

int Segment::Open() {
  if (_is_open) {
    TRAFT_LOG_ERROR("segment is already opened, fd: {}", _fd);
    return -1;
  }

  std::string open_path = GetOpenPath_();
  _fd = ::open(open_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (_fd < 0) {
    TRAFT_LOG_ERROR("failed to open segment: {}", open_path);
    return -1;
  }

  TRAFT_LOG_DEBUG("opened segment: {}", open_path);

  return 0;
}

uint32_t Segment::GetChecksum_(std::string_view sv) {
  return static_cast<uint32_t>(XXH3_64bits(sv.data(), sv.size()));
}

std::string Segment::GetOpenPath_() {
  return fmt::format("{}/log_inprogress_{:020}", _path, _first_index);
}

}  // namespace traft::storage
