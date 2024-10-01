#include "parsers.hpp"

#include <iostream>

namespace p2np::parsers {

enum class Ip4Type : uint8_t {
    TCP = 0x06,
};

struct __attribute__ ((packed)) IPv4Hdr {
    std::uint8_t version : 4;
    /// @brief Header length in 32but (4byte) words.
    std::uint8_t ihl : 4;
    std::uint8_t dscp : 6;
    std::uint8_t ecn : 2;
    /// @brief Length including the header in bytes.
    std::uint16_t length;
    std::uint16_t identification;
    std::uint16_t flags : 3;
    std::uint16_t fragmentOffset : 13;
    std::uint8_t ttl;
    Ip4Type protocol;
    std::uint16_t checksum;
    std::uint32_t srcAddress;
    std::uint32_t dstAddress;
};

bool ipv4(Packet &pkt) {
    if (pkt.data.size() < sizeof(IPv4Hdr)) {
        std::cerr
            << "error: Packet type is ipv4 but it doesn't fit ipv4 header.\n";
        return false;
    }

    auto header = reinterpret_cast<const IPv4Hdr *>(pkt.data.data());
    if (pkt.data.size() != header->length) {
        std::cerr << "warning: ipv4 length doesn't match the data length.\n";
    }

    pkt.srcAddress = header->srcAddress;
    pkt.dstAddress = header->dstAddress;

    if (pkt.data.size() < header->ihl) {
        std::cerr << "warning: ipv4 header ihl doesn't fit to data.\n";
        pkt.data = pkt.data.subspan(pkt.data.size());
    } else {
        pkt.data = pkt.data.subspan(header->ihl * 4);
    }

    switch (header->protocol) {
        case Ip4Type::TCP:
            return tcp(pkt);
        default:
            return false;
    }
}

} // namespace p2np::parsers

