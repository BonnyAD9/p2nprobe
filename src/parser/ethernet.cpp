#include <array>
#include <iostream>

#include "../endian.hpp"
#include "parsers.hpp"

namespace p2np::parsers {

enum class EthType : std::uint16_t {
    IPV4 = 0x0800,
};

inline EthType from_be(EthType v) {
    return static_cast<EthType>(p2np::from_be(static_cast<std::uint16_t>(v)));
}

constexpr std::size_t MAC_BYTES = 6;

struct __attribute__((packed)) EthHeader {
    std::array<char, MAC_BYTES> dst_mac;
    std::array<char, MAC_BYTES> src_mac;
    EthType type;
};

bool ethernet(storage::Packet &pkt, std::span<const char> data) {
    if (data.size() < sizeof(EthHeader)) {
        std::cerr
            << "error: Packet data is too small to fit ethernet header.`n";
        return false;
    }

    auto header = reinterpret_cast<const EthHeader *>(data.data());
    data = data.subspan(sizeof(EthHeader));

    pkt.l3_bytes = data.size();

    switch (from_be(header->type)) {
    case EthType::IPV4:
        return ipv4(pkt, data);
    default:
        return false;
    }
}

} // namespace p2np::parsers
