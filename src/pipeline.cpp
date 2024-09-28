#include "pipeline.hpp"

#include <iostream>

namespace p2np {

void Pipeline::process(Packet &pkt) {
    std::cout << "time: " << pkt.timestamp << " len: " << pkt.data.size() << std::endl;
    m_totalSize += pkt.data.size();
    ++m_totalPackets;
}

} // namespace p2np
