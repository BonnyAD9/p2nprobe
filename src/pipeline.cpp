#include "pipeline.hpp"

#include <iostream>

namespace p2np {

void Pipeline::process(Packet &pkt) {
    if (!pkt.parse()) {
        return;
    }

    _total_size += pkt.data.size();
    ++_total_packets;

    std::cout << " src: " << pkt.src_address << " dst: " << pkt.dst_address
              << '\n';
}

} // namespace p2np
