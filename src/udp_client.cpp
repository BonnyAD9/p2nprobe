#include "udp_client.hpp"

#include <cstring>
#include <stdexcept>

namespace p2np {

UdpClient::UdpClient(std::string address, std::uint16_t port) {
    addrinfo *res;
    const int ret = getaddrinfo(
        address.c_str(), std::to_string(port).c_str(), nullptr, &res
    );
    if (ret != 0) {
        throw std::runtime_error(
            "Failed to resolve the address '" + address +
            "': " + gai_strerror(ret)
        );
    }
    _addrlen = res->ai_addrlen;

    _address = std::vector<char>(_addrlen);
    auto addr_src = reinterpret_cast<const char *>(res->ai_addr);
    std::copy(addr_src, addr_src + _addrlen, address.data());

    freeaddrinfo(res);
    _fd = socket(res->ai_family, SOCK_DGRAM, 0);

    if (!_fd) {
        using std::string_literals::operator""s;
        std::array<char, 256> buf;
        const char *msg = strerror_r(errno, buf.data(), 256);
        throw std::runtime_error("Failed to open socket: "s + msg);
    }
}

void UdpClient::send(std::span<const char> data) const {
    sendto(_fd.get(), data.data(), data.size(), 0, address(), _addrlen);
}

const sockaddr *UdpClient::address() const {
    return reinterpret_cast<const sockaddr *>(_address.data());
}

} // namespace p2np