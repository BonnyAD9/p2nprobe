/// @author Jakub Antonín Štigler (xstigl00)

#include "pipeline.hpp"

namespace p2np {

void Pipeline::run() {
    _exporter->start();
    for (auto code = _src.next(); code != pcap::NextCode::END;
         code = _src.next()) {
        if (code != pcap::NextCode::SUCCESS) {
            continue;
        }
        process(_src.packet());
    }
    _exporter->send(_flows.drain());
    _exporter->stop();
}

void Pipeline::process(storage::Packet &pkt) {
    if (!pkt.parse()) {
        return;
    }

    _total_size += pkt.data.size();
    ++_total_packets;
    _flows.add(pkt);
    _exporter->send(_flows.exported(pkt.timestamp));
}

} // namespace p2np
