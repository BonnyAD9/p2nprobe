#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <span>

namespace p2np {

struct __attribute__((packed)) IpAddress {
public:
    IpAddress() = default;
    bool operator==(const IpAddress &) const = default;

    /// @brief Create ip address from ipv4 address.
    /// @param addr Ipv4 address.
    IpAddress(std::uint32_t addr);

    /// @brief The ip address.
    std::uint32_t address = 0;
};

/// @brief Print the ip address to the stream.
std::ostream &operator<<(std::ostream &os, IpAddress const &addr);

} // namespace p2np
