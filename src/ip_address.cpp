#include "ip_address.hpp"

#include <array>
#include <cerrno>
#include <cstring>
#include <ranges>
#include <stdexcept>
#include <string>

#include <arpa/inet.h>

namespace p2np {

IpAddress::IpAddress(std::uint32_t addr) : address(addr) { }

std::ostream &operator<<(std::ostream &os, p2np::IpAddress const &addr) {
    std::array<char, INET_ADDRSTRLEN> res;
    auto dst = inet_ntop(AF_INET, &addr, res.data(), res.size());
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
