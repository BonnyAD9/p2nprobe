#include "text.hpp"

#include <iostream>

namespace p2np::out {

void Text::send(std::vector<storage::Flow> flows) {
    for (auto &flow : flows) {
        send(flow);
    }
}

void Text::send(const storage::Flow &flow) {
    std::cout << flow.src_addr() << ':' << flow.src_port() << "->"
              << flow.dst_addr() << ':' << flow.dst_port() << ' '
              << flow.pkt_cnt() << '@' << flow.bytes() << " [" << flow.first()
              << "]..[" << flow.last() << "] " << (int)flow.protocol() << " "
              << (int)flow.tos() << '\n';
}

} // namespace p2np::out
