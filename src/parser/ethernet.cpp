#include <iostream>

#include "endian.hpp"
#include "parsers.hpp"

namespace p2np::parsers {

enum class EthType : std::uint16_t {
    IPV4 = 0x0800,
    IPV6 = 0x86dd,
};

inline EthType from_be(EthType v) {
    return static_cast<EthType>(from_be(static_cast<std::uint16_t>(v)));
}

struct __attribute__((packed)) EthHeader {
    char dstMac[6];
    char srcMac[6];
    EthType type;
};

bool ethernet(Packet &pkt, std::span<const char> data) {
    if (data.size() < sizeof(EthHeader)) {
        std::cerr
            << "error: Packet data is too small to fit ethernet header.`n";
        return false;
    }

    auto header = reinterpret_cast<const EthHeader *>(data.data());
    data = data.subspan(sizeof(EthHeader));

    switch (from_be(header->type)) {
    case EthType::IPV4:
        return ipv4(pkt, data);
    case EthType::IPV6:
        return ipv6(pkt, data);
    default:
        return false;
    }
}

} // namespace p2np::parsers
