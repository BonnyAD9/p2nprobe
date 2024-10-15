#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

#include <netdb.h>

#include "unique_fd.hpp"

namespace p2np {

class UdpClient {
public:
    UdpClient(std::string address, std::uint16_t port);

    void send(std::span<const char> data) const;

private:
    [[nodiscard]]
    const sockaddr *address() const;

    UniqueFd _fd;
    std::vector<char> _address;
    socklen_t _addrlen;
};

} // namespace p2np
