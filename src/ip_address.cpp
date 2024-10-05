#include "ip_address.hpp"

#include <array>
#include <cerrno>
#include <cstring>
#include <ranges>
#include <stdexcept>
#include <string>

#include <arpa/inet.h>

namespace p2np {

constexpr std::uint32_t IP4IN6 = 0xffff;

IpAddress::IpAddress(std::uint32_t ip4_addr) {
    m_address32 = { 0, 0, IP4IN6, ip4_addr };
}

IpAddress::IpAddress(std::array<char, 16> ip6_addr) { m_address = ip6_addr; }

bool IpAddress::is_v4() const {
    return m_address64[0] == 0 && m_address32[2] == IP4IN6;
}

std::ostream &operator<<(std::ostream &os, p2np::IpAddress const &m) {
    std::array<char, std::max(INET_ADDRSTRLEN, INET6_ADDRSTRLEN)> res;
    const void *adr = m.data();
    int af = AF_INET6;
    if (m.is_v4()) {
        adr = m.data_v4();
        af = AF_INET;
    }
    auto dst = inet_ntop(af, adr, res.data(), res.size());
    if (!dst) {
        using std::string_literals::operator""s;
        std::array<char, 256> buf;
        const char *msg = strerror_r(errno, buf.data(), 256);
        throw std::runtime_error("ineet_ntop() failed: "s + msg);
    }
    os << dst;

    return os;
}

} // namespace p2np
