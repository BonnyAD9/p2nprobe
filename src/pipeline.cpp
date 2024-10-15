#include "pipeline.hpp"

#include <iostream>

namespace p2np {

void Pipeline::run() {
    _exporter->start();
    for (auto code = _src.next(); code != NextCode::END; code = _src.next()) {
        if (code != NextCode::SUCCESS) {
            continue;
        }
        process(_src.packet());
    }
    _exporter->send(_flows.drain());
    _exporter->stop();

    std::cout << "Total packets: " << total_packets() << '\n'
              << "Total size   : " << total_size() << '\n'
              << "Avg size     : " << (total_size() / total_packets()) << '\n';
}

void Pipeline::process(Packet &pkt) {
    if (!pkt.parse()) {
        return;
    }

    _total_size += pkt.data.size();
    ++_total_packets;
    _flows.add(pkt);
    _exporter->send(_flows.exported(pkt.timestamp));
}

} // namespace p2np
