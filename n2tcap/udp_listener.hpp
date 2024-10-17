#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

#include <netdb.h>

#include "../src/unique_fd.hpp"

namespace n2tc {

using namespace p2np;

/// @brief Simple UDP client.
class UdpListener {
public:
    static constexpr std::size_t MTU = 2000;

    /// @brief Resolves the address and port and creates UDP socket.
    /// @param address Address of the server to connect to.
    /// @param port Port number of the server to connect to.
    UdpListener(const std::string &address, std::uint16_t port);

    /// @brief Send data to the server.
    std::span<const char> recv();

private:
    [[nodiscard]]
    const sockaddr *address() const;

    std::vector<char> _data;
    UniqueFd _fd;
};

} // namespace n2tc
