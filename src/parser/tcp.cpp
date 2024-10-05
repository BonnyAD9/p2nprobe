#include <cstdint>
#include <iostream>

#include "endian.hpp"
#include "parsers.hpp"

namespace p2np::parsers {

struct __attribute__((packed)) TcpHeader {
    static constexpr std::size_t DATA_OFFSET_SHIFT = 4;

    [[nodiscard]]
    std::uint16_t src_port() const {
        return from_be(src_port_be);
    }
    [[nodiscard]]
    std::uint16_t dst_port() const {
        return from_be(dst_port_be);
    }
    [[nodiscard]]
    unsigned data_offset() const {
        return (data_offset_and_reserved_be >> DATA_OFFSET_SHIFT) * 4;
    }

    std::uint16_t src_port_be;
    std::uint16_t dst_port_be;
    std::uint32_t seq_num_be;
    std::uint32_t ack_num_be;
    ///  0 1 2 3 4 5 6 7
    /// +-------+-------+
    /// | Off.  | Res.  |
    /// +-------+-------+
    std::uint8_t data_offset_and_reserved_be;
    std::uint8_t flags;
    std::uint16_t window_be;
    std::uint16_t checksum_be;
    std::uint16_t urgent_pointer_be;
};

bool tcp(Packet &pkt, std::span<const char> data) {
    if (data.size() < sizeof(TcpHeader)) {
        std::cerr << "TCP header doesn't fit the data size.\n";
        return false;
    }

    auto header = reinterpret_cast<const TcpHeader *>(data.data());

    pkt.src_port = header->src_port();
    pkt.dst_port = header->dst_port();
    pkt.tcp_flags = header->flags;

    const std::size_t off = header->data_offset();
    if (off < sizeof(TcpHeader)) {
        std::cerr << "TCP data includes header? " << off << "\n";
    }

    data = data.subspan(off);
    pkt.payload = data;
    return true;
}

} // namespace p2np::parsers
