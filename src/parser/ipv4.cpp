#include <iostream>

#include "endian.hpp"
#include "ip_type.hpp"
#include "parsers.hpp"

namespace p2np::parsers {

struct __attribute__((packed)) IPv4Hdr {
    std::uint8_t version : 4;
    /// @brief Header length in 32but (4byte) words.
    std::uint8_t ihl : 4;
    std::uint8_t dscp : 6;
    std::uint8_t ecn : 2;
    /// @brief Length including the header in bytes.
    std::uint16_t length;
    std::uint16_t identification;
    std::uint16_t flags : 3;
    std::uint16_t fragment_offset : 13;
    std::uint8_t ttl;
    IpType protocol;
    std::uint16_t checksum;
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
    if (data.size() < from_be(header->length)) {
        std::cerr << "warning: ipv4 length doesn't match the data length.\n";
    }

    pkt.src_address = header->src_address;
    pkt.dst_address = header->dst_address;

    if (data.size() < header->ihl) {
        std::cerr << "warning: ipv4 header ihl doesn't fit to data.\n";
        data = data.subspan(pkt.data.size());
    } else {
        data = data.subspan(header->ihl * 4);
    }

    switch (header->protocol) {
    case IpType::TCP:
        return tcp(pkt, data);
    default:
        return false;
    }
}

} // namespace p2np::parsers
