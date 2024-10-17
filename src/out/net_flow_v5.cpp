/// @author Jakub Antonín Štigler (xstigl00)

#include "net_flow_v5.hpp"

#include "../time.hpp"

namespace p2np::out {

std::int32_t to_ms(Duration dur) {
    using namespace std::chrono;
    return std::int32_t(duration_cast<milliseconds>(dur).count());
}

template<typename O, typename I> O clamp_cast(I in) {
    return O(std::min<I>(in, std::numeric_limits<O>::max()));
}

NetFlowV5Record::NetFlowV5Record(const storage::Flow &flow)
    : srcaddr(flow.src_addr()),
      dstaddr(flow.dst_addr()),
      d_pkts(flow.pkt_cnt()),
      d_octets(clamp_cast<std::uint32_t>(flow.bytes())),
      first(to_ms(flow.first() - sys_up())),
      last(to_ms(flow.last() - sys_up())),
      srcport(flow.src_port()),
      dstport(flow.dst_port()),
      tcp_flags(flow.tcp_flags()),
      prot(flow.protocol()),
      tos(flow.tos()) { }

NetFlowV5::NetFlowV5(UdpClient client, std::size_t mtu)
    : _data(std::min<std::size_t>(mtu, MAX_MTU)),
      _records(
          reinterpret_cast<NetFlowV5Record *>(
              _data.data() + sizeof(NetFlowV5Header)
          ),
          (_data.size() - sizeof(NetFlowV5Header)) / sizeof(NetFlowV5Record)
      ),
      _cur(_records.begin()),
      _header(reinterpret_cast<NetFlowV5Header *>(_data.data())),
      _client(std::move(client)) {
    _header->version = VERSION;
    _header->flow_sequence = 0;
    _header->engine_type = 0;
    _header->engine_id = 0;
    _header->sampling_interval = 0;
}

void NetFlowV5::send(std::vector<storage::Flow> flows) {
    send(std::span(flows));
}

void NetFlowV5::send(std::span<storage::Flow> flows) {
    while (!flows.empty()) {
        auto copy_cnt =
            std::min(flows.size(), std::size_t(_records.end() - _cur));

        std::copy_n(flows.begin(), copy_cnt, _cur);

        _cur += std::ptrdiff_t(copy_cnt);
        flows = flows.subspan(copy_cnt);

        if (_cur == _records.end()) {
            push();
        }
    }
}

void NetFlowV5::stop() { push(); }

void NetFlowV5::push() {
    auto flow_cnt = std::uint32_t(_cur - _records.begin());
    if (flow_cnt == 0) {
        return;
    }

    _header->count = flow_cnt;
    _header->sys_uptime = to_ms(sys_uptime());

    {
        using namespace std::chrono;
        auto now = Clock::now().time_since_epoch();
        auto now_s = duration_cast<seconds>(now);
        auto now_ns = duration_cast<nanoseconds>(now - now_s);
        _header->unix_secs = std::uint32_t(now_s.count());
        _header->unix_nsecs = std::uint32_t(now_ns.count());
    }

    _header->flow_sequence += flow_cnt;

    auto data_len =
        flow_cnt * sizeof(NetFlowV5Record) + sizeof(NetFlowV5Header);
    _client.send({ _data.data(), data_len });

    _cur = _records.begin();
}

} // namespace p2np::out
