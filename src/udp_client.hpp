#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

#include <netdb.h>

#include "unique_fd.hpp"

namespace p2np {

/// @brief Simple UDP client.
class UdpClient {
public:
    /// @brief Resolves the address and port and creates UDP socket.
    /// @param address Address of the server to connect to.
    /// @param port Port number of the server to connect to.
    UdpClient(const std::string &address, std::uint16_t port);

    /// @brief Send data to the server.
    void send(std::span<const char> data) const;

private:
    [[nodiscard]]
    const sockaddr *address() const;

    UniqueFd _fd;
    std::vector<char> _address;
    socklen_t _addrlen;
};

} // namespace p2np
