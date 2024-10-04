#include "pipeline.hpp"

#include "parser/parsers.hpp"

#include <iostream>

namespace p2np {

void Pipeline::process(Packet &pkt) {
    //std::cout << "time: " << pkt.timestamp << " len: " << pkt.data.size() << std::endl;
    m_totalSize += pkt.data.size();
    ++m_totalPackets;
    if (!parsers::ethernet(pkt)) {
        return;
    }

    std::cout << " src: " << pkt.srcAddress << " dst: " << pkt.dstAddress << std::endl;
}

} // namespace p2np
