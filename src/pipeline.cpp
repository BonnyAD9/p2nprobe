#include "pipeline.hpp"

#include <iostream>

#include "parser/parsers.hpp"

namespace p2np {

void Pipeline::process(Packet &pkt) {
    // std::cout << "time: " << pkt.timestamp << " len: " << pkt.data.size() <<
    // std::endl;
    m_totalSize += pkt.data.size();
    ++m_totalPackets;
    if (!parsers::parse(pkt)) {
        return;
    }

    std::cout << " src: " << pkt.srcAddress << " dst: " << pkt.dstAddress
              << std::endl;
}

} // namespace p2np
