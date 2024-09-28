#pragma once

#include <span>
#include <cstddef>
#include <chrono>

#include <sys/time.h>

namespace p2np {

/// @brief Holds data about packet.
struct Packet {
    Packet() = default;
    Packet(Packet &&) = default;
    Packet &operator=(Packet &&) = default;

    /// @brief Timestamp of the packet.
    std::chrono::system_clock::time_point timestamp;
    /// @brief Data in this packet.
    std::span<const char> data;
};

} // namespace p2np
