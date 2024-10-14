#pragma once

#include <chrono>

#include "ip_address.hpp"
#include "ip_protocol.hpp"
#include "packet.hpp"

namespace p2np {

/// @brief Unidirectional flow.
class Flow {
public:
    /// @brief Create new empty flow
    Flow() = default;

    /// @brief Add this packet to the flow statistics.
    ///
    /// This may be the first packet.
    /// @param pkt Packet to add to the flow statistics.
    void add(const Packet &pkt);

    /// @brief Get the source ip address of the flow.
    [[nodiscard]]
    IpAddress src_addr() const {
        return _src_addr;
    }

    /// @brief Get the destination ip address of the flow.
    [[nodiscard]]
    IpAddress dst_addr() const {
        return _dst_addr;
    }

    /// @brief Get the total number of packets in the flow.
    [[nodiscard]]
    std::uint32_t pkt_cnt() const {
        return _pkt_cnt;
    }

    /// @brief Get the total number of l3 bytes in the flow.
    [[nodiscard]]
    std::size_t bytes() const {
        return _bytes;
    }

    /// @brief Get the time of the first packet in the flow.
    [[nodiscard]]
    std::chrono::system_clock::time_point first() const {
        return _first;
    }

    /// @brief Get time of the last packet in the flow.
    [[nodiscard]]
    std::chrono::system_clock::time_point last() const {
        return _first;
    }

    /// @brief Get the source port of the flow.
    [[nodiscard]]
    std::uint16_t src_port() const {
        return _src_port;
    }

    /// @brief Get the destination port of the flow.
    [[nodiscard]]
    std::uint16_t dst_port() const {
        return _dst_port;
    }

    /// @brief Get the cumulative `or` of the tcp flags in the flow.
    /// @return
    [[nodiscard]]
    std::uint8_t tcp_flags() const {
        return _tcp_flags;
    }

    /// @brief Get the ip protocol of packets in the flow.
    /// @return
    [[nodiscard]]
    IpProtocol protocol() const {
        return _protocol;
    }

    /// @brief Get the ip type of service of the flow.
    /// @return
    [[nodiscard]]
    std::uint8_t tos() const {
        return _tos;
    }

private:
    IpAddress _src_addr;
    IpAddress _dst_addr;
    std::uint32_t _pkt_cnt = 0;
    std::size_t _bytes = 0;
    std::chrono::system_clock::time_point _first;
    std::chrono::system_clock::time_point _last;
    std::uint16_t _src_port = 0;
    std::uint16_t _dst_port = 0;
    std::uint8_t _tcp_flags = 0;
    IpProtocol _protocol{};
    std::uint8_t _tos = 0;
};

} // namespace p2np
