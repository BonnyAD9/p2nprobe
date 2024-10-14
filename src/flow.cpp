#include "flow.hpp"

namespace p2np {

void Flow::add(const Packet &pkt) {
    ++_pkt_cnt;
    _bytes += pkt.l3_bytes;
    _last = pkt.timestamp;
    _tcp_flags |= pkt.tcp_flags;

    if (_pkt_cnt != 1) {
        return;
    }

    // This is the first packet in this flow. Copy the additional information.
    _src_addr = pkt.src_address;
    _dst_addr = pkt.dst_address;
    _first = pkt.timestamp;
    _src_port = pkt.src_port;
    _dst_port = pkt.dst_port;
    _protocol = pkt.ip_protocol;
    _tos = pkt.ip_tos;
}

} // namespace p2np
