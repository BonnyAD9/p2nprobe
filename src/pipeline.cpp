#include "pipeline.hpp"

#include <iostream>

namespace p2np {

void Pipeline::process(Packet &pkt) {
    if (!pkt.parse()) {
        return;
    }

    _total_size += pkt.data.size();
    ++_total_packets;

    std::cout << "src='" << pkt.src_address << ':' << pkt.src_port << "' dst='"
              << pkt.dst_address << ':' << pkt.dst_port << "' proto='"
              << static_cast<int>(pkt.ip_protocol) << "' tos='"
              << static_cast<int>(pkt.ip_tos) << "' flags='"
              << static_cast<int>(pkt.tcp_flags) << "'\n";
}

} // namespace p2np
