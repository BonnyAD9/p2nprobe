#include "flow.hpp"

namespace p2np {

Flow::Flow(const Packet &pkt)
    : _src_addr(pkt.src_address),
      _dst_addr(pkt.dst_address),
      _bytes(pkt.l3_bytes),
      _first(pkt.timestamp),
      _last(pkt.timestamp),
      _src_port(pkt.src_port),
      _dst_port(pkt.dst_port),
      _tcp_flags(pkt.tcp_flags),
      _protocol(pkt.ip_protocol),
      _tos(pkt.ip_tos) { }

void Flow::add(const Packet &pkt) {
    ++_pkt_cnt;
    _bytes += pkt.l3_bytes;
    _last = pkt.timestamp;
    _tcp_flags |= pkt.tcp_flags;
}

} // namespace p2np
