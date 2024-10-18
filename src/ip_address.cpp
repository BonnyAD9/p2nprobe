/// @author Jakub Antonín Štigler (xstigl00)

#include "ip_address.hpp"

#include <array>
#include <cerrno>
#include <cstring>
#include <ranges>
#include <stdexcept>
#include <string>

#include <arpa/inet.h>

#include "strerror.hpp"

namespace p2np {

IpAddress::IpAddress(std::uint32_t addr) : address(addr) { }

std::ostream &operator<<(std::ostream &os, p2np::IpAddress const &addr) {
    std::array<char, INET_ADDRSTRLEN> res;
    auto dst = inet_ntop(AF_INET, &addr, res.data(), res.size());
    if (!dst) {
        throw std::runtime_error("ineet_ntop() failed: " + strerror());
    }
    os << dst;

    return os;
}

} // namespace p2np
