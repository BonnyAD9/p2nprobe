
#include "parsers.hpp"

#include <iostream>

namespace p2np::parsers {

enum class EthType : uint16_t {
    IPV4 = 0x0800,
    IPV6 = 0x86dd,
};

struct __attribute__ ((packed)) EthHeader {
    char dstMac[6];
    char srcMac[6];
    EthType type;
};

bool ethernet(Packet &pkt) {
    if (pkt.data.size() < sizeof(EthHeader)) {
        std::cerr
            << "error: Packet data is too small to fit ethernet header.`n";
        return false;
    }

    auto header = reinterpret_cast<const EthHeader *>(pkt.data.data());
    pkt.data = pkt.data.subspan(sizeof(EthHeader));

    switch (header->type) {
        case EthType::IPV4:
            ipv4(pkt);
            break;
        case EthType::IPV6:
            ipv6(pkt);
            break;
        default:
            return false;
    }
}

} // namespace p2np

