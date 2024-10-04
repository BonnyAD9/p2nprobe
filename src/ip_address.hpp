#pragma once

#include <cstdint>
#include <span>
#include <array>
#include <ostream>

namespace p2np {

class __attribute__ ((packed)) IpAddress {
public:
    IpAddress() = default;

    /// @brief Create ip address from ipv4 address.
    /// @param ipv4Addr Ipv4 address.
    IpAddress(std::uint32_t ipv4Addr);

    /// @brief Create ip address from ipv6 address data.
    /// @param ipAddress Ipv6 address.
    IpAddress(std::array<char, 16> ipv6Addr);

    /// @brief Checks if the address is ipv4
    /// @return `true` if the address is ipv4, otherwise false.
    bool isV4() const;

    const char *data() const { return m_address.data(); }
    const char *dataV4() const { return m_address.data() + 12; }

private:
    union __attribute__ ((packed)) {
        std::array<char, 16> m_address = {0};
        std::array<uint32_t, 4> m_address32;
        std::array<uint64_t, 2> m_address64;
    };
};

std::ostream &operator<<(std::ostream &os, IpAddress const &m);

} // namespace p2np

