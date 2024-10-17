/// @author Jakub Antonín Štigler (xstigl00)

#include "record_printer.hpp"

#include <iostream>

namespace n2tc {

Instant get_time(const NetFlowV5Header &h) {
    using namespace std::chrono;
    return Instant(duration_cast<Duration>(
        seconds(h.unix_secs) + nanoseconds(h.unix_nsecs)
    ));
}

void print(const NetFlowV5Records &l) {
    std::cout << "Header:\n";
    print(l.header());
    std::cout << "Flows:\n";
    auto time = get_time(l.header()) +
                std::chrono::milliseconds(l.header().sys_uptime);

    for (auto &r : l.records()) {
        if (r.prot != IpProtocol::TCP) {
            continue;
        }
        std::cout << "-----\n";
        print(r, time);
    }

    std::cout << '\n';
}

void print(const NetFlowV5Header &h) {
    auto time = get_time(h);

    const std::uint16_t sampling = h.sampling_interval;
    auto smode = sampling & 0x3;
    auto sint = sampling >> 2;

    std::cout << "  version   : " << h.version << "\n  count     : " << h.count
              << "\n  sys_uptime: " << h.sys_uptime << "ms"
              << "\n  time      : " << time
              << "\n  flow seq  : " << h.flow_sequence
              << "\n  eng type  : " << int(h.engine_type)
              << "\n  eng id    : " << int(h.engine_id)
              << "\n  sampling  : " << smode << ':' << sint << "\n";
}

void print(const NetFlowV5Record &r, Instant time) {

    std::cout << "  src     : " << r.srcaddr << ':' << r.srcport
              << "\n  dst     : " << r.dstaddr << ':' << r.dstport
              << "\n  hop     : " << r.nexthop << "\n  input   : " << r.input
              << "\n  output  : " << r.output << "\n  pkts    : " << r.d_pkts
              << "\n  data    : " << r.d_octets << "\n  first   : " << r.first
              << " [" << time + std::chrono::milliseconds(r.first) << ']'
              << "\n  last    : " << r.last << " ["
              << time + std::chrono::milliseconds(r.last) << ']'
              << "\n  flags   : " << int(r.tcp_flags)
              << "\n  prot    : " << int(r.prot)
              << "\n  tos     : " << int(r.tos) << "\n  src as  : " << r.src_as
              << "\n  dst as  : " << r.dst_as
              << "\n  src mask: " << int(r.src_mask)
              << "\n  dst mask: " << int(r.dst_mask) << "\n";
}

} // namespace n2tc
