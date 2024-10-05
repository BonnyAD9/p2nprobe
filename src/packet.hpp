#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <span>

#include <sys/time.h>

#include "ip_address.hpp"
#include "ip_protocol.hpp"

namespace p2np {

/// @brief Holds data about packet.
struct Packet {
    Packet() = default;
    Packet(Packet &&) = default;
    Packet &operator=(Packet &&) = default;

    /// @brief Extract data from the packet.
    /// @returns `true` on successfully parsed TCP packets.
    bool parse();

    std::size_t l3_bytes = 0;

    IpAddress src_address;
    IpAddress dst_address;
    IpProtocol ip_protocol;
    std::uint8_t ip_tos;

    std::uint16_t src_port = 0;
    std::uint16_t dst_port = 0;
    std::uint8_t tcp_flags = 0;

    /// @brief Timestamp of the packet.
    std::chrono::system_clock::time_point timestamp{
        std::chrono::system_clock::duration(0)
    };
    /// @brief Data in this packet.
    std::span<const char> data;
    /// @brief TCP payload data.
    std::span<const char> payload;
};

} // namespace p2np
