#include "parsers.hpp"

#include <iostream>
#include <optional>

#include "../ip_address.hpp"
#include "ip_type.hpp"

namespace p2np::parsers {

struct __attribute__ ((packed)) IPv6Header {
    std::uint32_t version : 4;
    std::uint32_t trafficClass : 8;
    std::uint32_t flowLabel : 20;
    /// @brief Length of the payload including extension headers (not including
    /// this header) in octets (64-bit/8-byte/2^3-byte words)
    std::uint16_t payloadLength;
    IpType nextHeader;
    std::uint8_t hopLimit;
    IpAddress srcAddress;
    IpAddress dstAddress;
};

struct __attribute__ ((packed)) FragmentHdr {
    IpType nextHeader;
    std::uint8_t reserved;
    std::uint16_t fragmentOffset;
    std::uint32_t identification;
};

struct __attribute__ ((packed)) AuthHdr {
    IpType nextHeader;
    std::uint8_t length;
    std::uint16_t reserved;
    std::uint32_t spi;
    std::uint32_t seqNum;
};

struct __attribute__ ((packed)) ExtHdr {
    IpType nextHeader;
    std::uint8_t length;
    char data[6];
};

static std::optional<IpType> skip_headers(Packet &pkt, std::size_t &hlen, IpType type);

template<typename H, typename F>
std::optional<IpType> skip_hdr(Packet &pkt, std::size_t &hlen, F lenMap);
std::optional<IpType> skip_ext_hdr(Packet &pkt, std::size_t &hlen);

static std::optional<IpType> hop_by_hop(Packet &pkt, std::size_t &hlen);
static std::optional<IpType> routing(Packet &pkt, std::size_t &hlen);
static std::optional<IpType> fragment(Packet &pkt, std::size_t &hlen);
static std::optional<IpType> auth(Packet &pkt, std::size_t &hlen);
static std::optional<IpType> esp(Packet &pkt, std::size_t &hlen);
static std::optional<IpType> dst_opts(Packet &pkt, std::size_t &hlen);
static std::optional<IpType> mobility(Packet &pkt, std::size_t &hlen);
static std::optional<IpType> host_id(Packet &pkt, std::size_t &hlen);
static std::optional<IpType> shim6(Packet &pkt, std::size_t &hlen);

bool ipv6(Packet &pkt) {
    if (pkt.data.size() < sizeof(IPv6Header)) {
        std::cerr << "IPv6 packet too short for IPv6 header\n";
        return false;
    }

    auto header = reinterpret_cast<const IPv6Header *>(pkt.data.data());
    pkt.data = pkt.data.subspan(sizeof(IPv6Header));

    pkt.srcAddress = header->srcAddress;
    pkt.dstAddress = header->dstAddress;

    std::size_t hlen = 0;
    auto typ = skip_headers(pkt, hlen, header->nextHeader);

    if (!typ) {
        return false;
    }

    switch (*typ) {
        case IpType::TCP:
            return tcp(pkt);
        default:
            return false;
    }
}

static std::optional<IpType> skip_headers(Packet &pkt, std::size_t &hlen, IpType type) {
    std::optional<IpType> next_type = type;
    while (next_type) {
        switch (*next_type) {
            case IpType::HOP_BY_HOP_HEADER:
                next_type = hop_by_hop(pkt, hlen);
                break;
            case IpType::ROUTING_HEADER:
                next_type = routing(pkt, hlen);
                break;
            case IpType::FRAGMENT_HEADER:
                next_type = fragment(pkt, hlen);
                break;
            case IpType::AUTH_HEADER:
                next_type = auth(pkt, hlen);
                break;
            case IpType::ESP_HEADER:
                next_type = esp(pkt, hlen);
                break;
            case IpType::DST_OPTS_HEADER:
                next_type = dst_opts(pkt, hlen);
                break;
            case IpType::MOBILITY_HEADER:
                next_type = mobility(pkt, hlen);
                break;
            case IpType::HOST_ID_HEADER:
                next_type = host_id(pkt, hlen);
                break;
            case IpType::SHIM6_HEADER:
                next_type = shim6(pkt, hlen);
                break;
            default:
                return next_type;
        }
    }

    return std::nullopt;
}

template<typename H, typename F>
std::optional<IpType> skip_hdr(Packet &pkt, std::size_t &hlen, F lenMap) {
    if (pkt.data.size() < sizeof(H)) {
        std::cerr
            << "IPv6 packet too short for IPv6 extension header header\n";
        return std::nullopt;
    }

    auto header = reinterpret_cast<const H *>(pkt.data.data());

    hlen += sizeof(H);
    pkt.data = pkt.data.subspan(sizeof(H));
    auto skip_size = static_cast<std::size_t>(lenMap(header));

    if (pkt.data.size() < skip_size) {
        std::cerr << "IPv6 packet too short for IPv6 extension header data\n";
        return std::nullopt;
    }
    hlen += skip_size;
    pkt.data = pkt.data.subspan(skip_size);

    return header->nextHeader;
}

std::optional<IpType> skip_ext_hdr(Packet &pkt, std::size_t &hlen) {
    return skip_hdr<ExtHdr>(pkt, hlen, [=](auto h) { return h->length << 3; });
}

static std::optional<IpType> hop_by_hop(Packet &pkt, std::size_t &hlen) {
    return skip_ext_hdr(pkt, hlen);
}

static std::optional<IpType> routing(Packet &pkt, std::size_t &hlen) {
    return skip_ext_hdr(pkt, hlen);
}

static std::optional<IpType> fragment(Packet &pkt, std::size_t &hlen) {
    return skip_hdr<FragmentHdr>(pkt, hlen, [=](auto) { return 0; });
}

static std::optional<IpType> auth(Packet &pkt, std::size_t &hlen) {
    return skip_hdr<AuthHdr>(pkt, hlen, [=](auto h) { return h->length * 4 - 1; });
}

static std::optional<IpType> esp(Packet &, std::size_t &) {
    return std::nullopt;
}

static std::optional<IpType> dst_opts(Packet &pkt, std::size_t &hlen) {
    return skip_ext_hdr(pkt, hlen);
}

static std::optional<IpType> mobility(Packet &pkt, std::size_t &hlen) {
    return skip_ext_hdr(pkt, hlen);
}

static std::optional<IpType> host_id(Packet &pkt, std::size_t &hlen) {
    return skip_ext_hdr(pkt, hlen);
}

static std::optional<IpType> shim6(Packet &pkt, std::size_t &hlen) {
    return skip_ext_hdr(pkt, hlen);
}


} // namespace p2np::parsers

