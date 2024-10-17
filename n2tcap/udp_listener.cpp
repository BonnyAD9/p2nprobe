/// @author Jakub Antonín Štigler (xstigl00)

#include "udp_listener.hpp"

#include <cstring>
#include <iostream>
#include <stdexcept>

namespace n2tc {

UdpListener::UdpListener(const std::string &address, std::uint16_t port)
    : _data(MTU) {
    addrinfo *res;
    int ret = getaddrinfo(
        address.c_str(), std::to_string(port).c_str(), nullptr, &res
    );
    if (ret != 0) {
        throw std::runtime_error(
            "Failed to resolve the address '" + address +
            "': " + gai_strerror(ret)
        );
    }

    _fd = socket(res->ai_family, SOCK_DGRAM, 0);
    if (!_fd) {
        using std::string_literals::operator""s;
        std::array<char, 256> buf;
        const char *msg = strerror_r(errno, buf.data(), 256);
        throw std::runtime_error("Failed to open socket: "s + msg);
    }

    ret = bind(_fd.get(), res->ai_addr, res->ai_addrlen);
    if (ret < 0) {
        using std::string_literals::operator""s;
        std::array<char, 256> buf;
        const char *msg = strerror_r(errno, buf.data(), 256);
        ;
        std::cerr << "Failed to bind: " << _fd.get() << ": " << msg << '\n';
    }

    freeaddrinfo(res);
}

std::span<const char> UdpListener::recv() {
    auto r = ::recv(_fd.get(), _data.data(), MTU, MSG_WAITALL);
    if (r < 0) {
        using std::string_literals::operator""s;
        std::array<char, 256> buf;
        const char *msg = strerror_r(errno, buf.data(), 256);
        ;
        std::cerr << "Failed to send data on " << _fd.get() << ": " << msg
                  << '\n';
    }

    return { _data.data(), std::size_t(r) };
}

} // namespace n2tc
