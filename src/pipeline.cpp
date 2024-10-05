#include "pipeline.hpp"

#include <iostream>

#include "parser/parsers.hpp"

namespace p2np {

void Pipeline::process(Packet &pkt) {
    // std::cout << "time: " << pkt.timestamp << " len: " << pkt.data.size() <<
    // std::endl;
    _total_size += pkt.data.size();
    ++_total_packets;
    if (!parsers::parse(pkt)) {
        return;
    }

    std::cout << " src: " << pkt.src_address << " dst: " << pkt.dst_address
              << '\n';
}

} // namespace p2np
