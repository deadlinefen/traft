#pragma once

#include <cstdint>
#include <string_view>


namespace traft::storage {

std::uint64_t GetChecksum64(std::string_view sv);

std::uint32_t GetChecksum32(std::string_view sv);

bool VerifyChecksum64(std::string_view sv, std::uint64_t checksum);

bool VerifyChecksum32(std::string_view sv, std::uint32_t checksum);

}
