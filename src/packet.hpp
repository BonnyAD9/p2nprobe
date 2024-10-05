#pragma once

#include <chrono>
#include <cstddef>
#include <span>

#include <sys/time.h>

#include "ip_address.hpp"

namespace p2np {

/// @brief Holds data about packet.
struct Packet {
    Packet() = default;
    Packet(Packet &&) = default;
    Packet &operator=(Packet &&) = default;

    /// @brief Extract data from the packet.
    void parse();

    IpAddress srcAddress;
    IpAddress dstAddress;

    /// @brief Timestamp of the packet.
    std::chrono::system_clock::time_point timestamp;
    /// @brief Data in this packet.
    std::span<const char> data;
};

} // namespace p2np
