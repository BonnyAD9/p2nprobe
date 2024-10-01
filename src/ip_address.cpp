#include "ip_address.hpp"

#include <ranges>

namespace p2np {

IpAddress::IpAddress(std::uint32_t ipv4Adr) {
    m_address32 = { 0, 0, 0xffff, ipv4Adr };
}

IpAddress::IpAddress(std::array<char, 16> ipv6Addr) {
    m_address = ipv6Addr;
}

bool IpAddress::isV4() const {
    return m_address64[0] == 0 && m_address32[3] == 0xffff;
}

} // namespace p2np
