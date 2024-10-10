#pragma once

#include "ip_address.hpp"
#include "ip_protocol.hpp"
#include "packet.hpp"

namespace p2np {

/// @brief Hashable flow key.
struct __attribute__((packed)) FlowKey {
    bool operator==(const FlowKey &) const = default;

    /// @brief Extract the flow key from packet.
    /// @param pkt Packet to extract the flow key from.
    FlowKey(Packet &pkt)
        : src_adr(pkt.src_address),
          dst_adr(pkt.dst_address),
          src_port(pkt.src_port),
          dst_port(pkt.dst_port),
          protocol(pkt.ip_protocol),
          tos(pkt.ip_tos) { }

    IpAddress src_adr = 0;
    IpAddress dst_adr = 0;
    std::uint16_t src_port = 0;
    std::uint16_t dst_port = 0;
    IpProtocol protocol;
    std::uint8_t tos;
    std::uint16_t pad = 0;
};

} // namespace p2np

template<> struct std::hash<p2np::FlowKey> {
    std::size_t operator()(const p2np::FlowKey &key) const noexcept {
        auto data = reinterpret_cast<const char *>(&key);
        return std::hash<std::string_view>()({ data, sizeof(key) });
    }
};
