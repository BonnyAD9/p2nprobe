#include <iostream>
#include <optional>

#include "../ip_address.hpp"
#include "ip_type.hpp"
#include "parsers.hpp"

namespace p2np::parsers {

struct __attribute__((packed)) IPv6Header {
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

struct __attribute__((packed)) FragmentHdr {
    IpType nextHeader;
    std::uint8_t reserved;
    std::uint16_t fragmentOffset;
    std::uint32_t identification;
};

struct __attribute__((packed)) AuthHdr {
    IpType nextHeader;
    std::uint8_t length;
    std::uint16_t reserved;
    std::uint32_t spi;
    std::uint32_t seqNum;
};

struct __attribute__((packed)) ExtHdr {
    IpType nextHeader;
    std::uint8_t length;
    char data[6];
};

static std::optional<IpType> parse_headers(
    Packet &pkt, std::span<const char> &data, IpType type
);

template<typename H, typename F>
std::optional<IpType> skip_hdr(std::span<const char> &data, F lenMap);
static std::optional<IpType> skip_ext_hdr(std::span<const char> &data);

static std::optional<IpType> hop_by_hop(
    Packet &pkt, std::span<const char> &data
);
static std::optional<IpType> routing(Packet &pkt, std::span<const char> &data);
static std::optional<IpType> fragment(
    Packet &pkt, std::span<const char> &data
);
static std::optional<IpType> auth(Packet &pkt, std::span<const char> &data);
static std::optional<IpType> esp(Packet &pkt, std::span<const char> &data);
static std::optional<IpType> dst_opts(
    Packet &pkt, std::span<const char> &data
);
static std::optional<IpType> mobility(
    Packet &pkt, std::span<const char> &data
);
static std::optional<IpType> host_id(Packet &pkt, std::span<const char> &data);
static std::optional<IpType> shim6(Packet &pkt, std::span<const char> &data);

bool ipv6(Packet &pkt, std::span<const char> data) {
    if (data.size() < sizeof(IPv6Header)) {
        std::cerr << "IPv6 packet too short for IPv6 header\n";
        return false;
    }

    auto header = reinterpret_cast<const IPv6Header *>(data.data());
    data = data.subspan(sizeof(IPv6Header));

    pkt.srcAddress = header->srcAddress;
    pkt.dstAddress = header->dstAddress;

    auto typ = parse_headers(pkt, data, header->nextHeader);

    if (!typ) {
        return false;
    }

    switch (*typ) {
    case IpType::TCP:
        return tcp(pkt, data);
    default:
        return false;
    }
}

static std::optional<IpType> parse_headers(
    Packet &pkt, std::span<const char> &data, IpType type
) {
    std::optional<IpType> next_type = type;
    while (next_type) {
        switch (*next_type) {
        case IpType::HOP_BY_HOP_HEADER:
            next_type = hop_by_hop(pkt, data);
            break;
        case IpType::ROUTING_HEADER:
            next_type = routing(pkt, data);
            break;
        case IpType::FRAGMENT_HEADER:
            next_type = fragment(pkt, data);
            break;
        case IpType::AUTH_HEADER:
            next_type = auth(pkt, data);
            break;
        case IpType::ESP_HEADER:
            next_type = esp(pkt, data);
            break;
        case IpType::DST_OPTS_HEADER:
            next_type = dst_opts(pkt, data);
            break;
        case IpType::MOBILITY_HEADER:
            next_type = mobility(pkt, data);
            break;
        case IpType::HOST_ID_HEADER:
            next_type = host_id(pkt, data);
            break;
        case IpType::SHIM6_HEADER:
            next_type = shim6(pkt, data);
            break;
        default:
            return next_type;
        }
    }

    return std::nullopt;
}

template<typename H, typename F>
std::optional<IpType> skip_hdr(std::span<const char> &data, F lenMap) {
    if (data.size() < sizeof(H)) {
        std::cerr
            << "IPv6 packet too short for IPv6 extension header header\n";
        return std::nullopt;
    }

    auto header = reinterpret_cast<const H *>(data.data());

    data = data.subspan(sizeof(H));
    auto skip_size = static_cast<std::size_t>(lenMap(header));

    if (data.size() < skip_size) {
        std::cerr << "IPv6 packet too short for IPv6 extension header data\n";
        return std::nullopt;
    }
    data = data.subspan(skip_size);

    return header->nextHeader;
}

static std::optional<IpType> skip_ext_hdr(std::span<const char> &data) {
    return skip_hdr<ExtHdr>(data, [=](auto h) { return h->length << 3; });
}

static std::optional<IpType> hop_by_hop(
    Packet &, std::span<const char> &data
) {
    return skip_ext_hdr(data);
}

static std::optional<IpType> routing(Packet &, std::span<const char> &data) {
    return skip_ext_hdr(data);
}

static std::optional<IpType> fragment(Packet &, std::span<const char> &data) {
    return skip_hdr<FragmentHdr>(data, [=](auto) { return 0; });
}

static std::optional<IpType> auth(Packet &, std::span<const char> &data) {
    return skip_hdr<AuthHdr>(data, [=](auto h) { return h->length * 4 - 1; });
}

static std::optional<IpType> esp(Packet &, std::span<const char> &) {
    return std::nullopt;
}

static std::optional<IpType> dst_opts(Packet &, std::span<const char> &data) {
    return skip_ext_hdr(data);
}

static std::optional<IpType> mobility(Packet &, std::span<const char> &data) {
    return skip_ext_hdr(data);
}

static std::optional<IpType> host_id(Packet &, std::span<const char> &data) {
    return skip_ext_hdr(data);
}

static std::optional<IpType> shim6(Packet &, std::span<const char> &data) {
    return skip_ext_hdr(data);
}

} // namespace p2np::parsers