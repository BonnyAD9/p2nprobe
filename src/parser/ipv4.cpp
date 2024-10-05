#include <iostream>

#include "endian.hpp"
#include "parsers.hpp"

namespace p2np::parsers {

enum class IpType : std::uint8_t {
    HOP_BY_HOP_HEADER = 0,
    TCP = 0x06,
    ROUTING_HEADER = 46,
    FRAGMENT_HEADER = 44,
    ESP_HEADER = 50,
    AUTH_HEADER = 51,
    DST_OPTS_HEADER = 60,
    MOBILITY_HEADER = 135,
    HOST_ID_HEADER = 139,
    SHIM6_HEADER = 140,
};

struct __attribute__((packed)) IPv4Hdr {
    static constexpr std::size_t IHL_MASK = 0x0f;

    [[nodiscard]]
    unsigned ihl() const {
        return (version_and_ihl & IHL_MASK) * 4;
    }
    /// @brief Length including the header in bytes.
    [[nodiscard]]
    std::uint16_t length() const {
        return from_be(length_be);
    }

    ///  0 1 2 3 4 5 6 7
    /// +-------+-------+
    /// | Ver.  | IHL   |
    /// +-------+-------+
    std::uint8_t version_and_ihl;
    ///  0 1 2 3 4 5 6 7
    /// +-----------+---+
    /// | DSCP      | E |
    /// +-----------+---+
    std::uint8_t dscp_and_ecn;
    /// @brief Header length in 32bit (4byte) words.
    /// @brief Length including the header in bytes.
    std::uint16_t length_be;
    std::uint16_t identification_be;
    ///  0 1 2 3 4 5 6 7 8 9 A B C D E F
    /// +-----+-------------------------+
    /// | Fl. | Fragment offset         |
    /// +-----+-------------------------+
    std::uint16_t flags_and_fragment_offset_be;
    std::uint8_t ttl;
    IpType protocol;
    std::uint16_t checksum_be;
    std::uint32_t src_address;
    std::uint32_t dst_address;
};

bool ipv4(Packet &pkt, std::span<const char> data) {
    if (data.size() < sizeof(IPv4Hdr)) {
        std::cerr
            << "error: Packet type is ipv4 but it doesn't fit ipv4 header.\n";
        return false;
    }

    auto header = reinterpret_cast<const IPv4Hdr *>(data.data());
    if (data.size() < header->length()) {
        std::cerr << "warning: ipv4 length doesn't match the data length.\n";
    }

    pkt.src_address = header->src_address;
    pkt.dst_address = header->dst_address;

    if (data.size() < header->ihl()) {
        std::cerr << "warning: ipv4 header ihl doesn't fit to data.\n";
        data = data.subspan(pkt.data.size());
    } else {
        data = data.subspan(header->ihl());
    }

    switch (header->protocol) {
    case IpType::TCP:
        return tcp(pkt, data);
    default:
        return false;
    }
}

} // namespace p2np::parsers
