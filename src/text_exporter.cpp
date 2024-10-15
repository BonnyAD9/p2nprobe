#include "text_exporter.hpp"

#include <iostream>

namespace p2np {

void TextExporter::send(std::vector<Flow> flows) {
    for (auto &flow : flows) {
        send(flow);
    }
}

void TextExporter::send(const Flow &flow) {
    std::cout << flow.src_addr() << ':' << flow.src_port() << "->"
              << flow.dst_addr() << ':' << flow.dst_port() << ' '
              << flow.pkt_cnt() << '@' << flow.bytes() << " [" << flow.first()
              << "]..[" << flow.last() << "] " << (int)flow.protocol() << " "
              << (int)flow.tos() << '\n';
}

} // namespace p2np
