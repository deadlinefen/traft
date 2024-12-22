#pragma once

#include <netinet/in.h>
#include <cstdint>

#include "fmt/format.h"

namespace traft::utils {

class EndPoint {
 public:
  EndPoint() = default;
  EndPoint(uint32_t ip, int port) : ip_{ip}, port_{port} {}
  EndPoint(const sockaddr_in &in)
      : ip_{ntohl(in.sin_addr.s_addr)}, port_{ntohs(in.sin_port)} {}

  EndPoint(const EndPoint &) = default;
  EndPoint(EndPoint &&) = default;

  EndPoint &operator=(const EndPoint &) = default;
  auto operator<=>(const EndPoint &) const = default;

  bool IsAny() const { return ip_ == 0; }

  uint32_t Ip() const { return ip_; }
  int Port() const { return port_; }

  std::string Describe() const { return fmt::format("endpoint{{{}}}", ToString()); }
  std::string ToString() const {
    const uint8_t *ip = reinterpret_cast<const uint8_t *>(&ip_);
    return fmt::format("{}.{}.{}.{}:{}", static_cast<int>(ip[0]), static_cast<int>(ip[1]),
                       static_cast<int>(ip[2]), static_cast<int>(ip[3]), port_);
  }

 private:
  uint32_t ip_{0};
  int port_{0};
};

}  // namespace traft::utils
