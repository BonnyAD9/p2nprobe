#include "ip_address.hpp"

#include <ranges>
#include <array>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>

#include <arpa/inet.h>

namespace p2np {

IpAddress::IpAddress(std::uint32_t ipv4Adr) {
    m_address32 = { 0, 0, 0xffff, ipv4Adr };
}

IpAddress::IpAddress(std::array<char, 16> ipv6Addr) {
    m_address = ipv6Addr;
}

bool IpAddress::isV4() const {
    return m_address64[0] == 0 && m_address32[2] == 0xffff;
}

std::ostream &operator<<(std::ostream &os, p2np::IpAddress const &m) {
    std::array<char, std::max(INET_ADDRSTRLEN, INET6_ADDRSTRLEN)> res;
    const void *adr = m.data();
    int af = AF_INET6;
    if (m.isV4()) {
        adr = m.dataV4();
        af = AF_INET;
    }
    auto dst = inet_ntop(af, adr, res.data(), res.size());
    if (!dst) {
        using std::string_literals::operator""s;
        const char *msg = strerror(errno);
        throw std::runtime_error("ineet_ntop() failed: "s + msg);
    }
    os << dst;

    return os;
}

} // namespace p2np
