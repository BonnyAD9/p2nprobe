#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <span>

namespace p2np {

class __attribute__((packed)) IpAddress {
public:
    IpAddress() = default;

    /// @brief Create ip address from ipv4 address.
    /// @param ip4_addr Ipv4 address.
    IpAddress(std::uint32_t ip4_addr);

    /// @brief Create ip address from ipv6 address data.
    /// @param ip6_addr Ipv6 address.
    IpAddress(std::array<char, 16> ip6_addr);

    /// @brief Checks if the address is ipv4
    /// @return `true` if the address is ipv4, otherwise false.
    [[nodiscard]]
    bool is_v4() const;

    [[nodiscard]]
    const char *data() const {
        return m_address.data();
    }
    [[nodiscard]]
    const char *data_v4() const {
        return m_address.data() + V4_OFFSET;
    }

    static constexpr std::size_t V4_OFFSET = 12;

private:
    union __attribute__((packed)) {
        std::array<char, 16> m_address = { 0 };
        std::array<uint32_t, 4> m_address32;
        std::array<uint64_t, 2> m_address64;
    };
};

std::ostream &operator<<(std::ostream &os, IpAddress const &m);

} // namespace p2np
