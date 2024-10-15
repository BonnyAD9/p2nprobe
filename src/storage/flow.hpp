#pragma once

#include "../ip_address.hpp"
#include "../ip_protocol.hpp"
#include "../time.hpp"
#include "packet.hpp"

namespace p2np::storage {

/// @brief Unidirectional flow.
class Flow {
public:
    /// @brief Create new empty flow
    Flow(const Packet &pkt);

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
    Instant first() const {
        return _first;
    }

    /// @brief Get time of the last packet in the flow.
    [[nodiscard]]
    Instant last() const {
        return _last;
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

    bool &is_finished() { return _finished; }

private:
    IpAddress _src_addr;
    IpAddress _dst_addr;
    std::uint32_t _pkt_cnt = 1;
    std::size_t _bytes;
    Instant _first;
    Instant _last;
    std::uint16_t _src_port;
    std::uint16_t _dst_port;
    std::uint8_t _tcp_flags;
    IpProtocol _protocol;
    std::uint8_t _tos;

    bool _finished;
};

} // namespace p2np::storage
