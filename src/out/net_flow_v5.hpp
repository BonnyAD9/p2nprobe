#pragma once

#include <string>

#include <netdb.h>

#include "../endian.hpp"
#include "../udp_client.hpp"
#include "exporter.hpp"

namespace p2np::out {

struct __attribute__((packed)) NetFlowV5Header {
    std::uint8_t version;
    Be<std::uint16_t> count;
    /// @brief System uptime in milliseconds
    Be<std::uint32_t> sys_uptime;
    Be<std::uint32_t> unix_secs;
    Be<std::uint32_t> unix_nsecs;
    Be<std::uint32_t> flow_sequence;
    std::uint8_t engine_type;
    std::uint8_t engine_id;
    Be<std::uint16_t> sampling_interval;
};

struct __attribute__((packed)) NetFlowV5Record {
    IpAddress srcaddr;
    IpAddress dstaddr;
    IpAddress nexthop;
    Be<std::uint16_t> input;
    Be<std::uint16_t> output;
    Be<std::uint32_t> d_pkts;
    Be<std::uint32_t> d_octets;
    Be<std::uint32_t> first;
    Be<std::uint32_t> last;
    Be<std::uint16_t> srcport;
    Be<std::uint16_t> dstport;
    std::uint8_t pad1;
    std::uint8_t tcp_flags;
    std::uint8_t prot;
    std::uint8_t tos;
    Be<std::uint16_t> src_as;
    Be<std::uint16_t> dst_as;
    std::uint8_t src_mask;
    std::uint8_t dst_mask;
    std::uint16_t pad2;
};

class NetFlowV5 : public Exporter {
public:
    static constexpr std::size_t DEFAULT_MTU = 1500 - 8 - 40;

    NetFlowV5(UdpClient client, std::size_t mtu = DEFAULT_MTU)
        : _mtu(mtu), _client(std::move(client)) {
        (void)_mtu;
    }

    void start() override {};

    void send(std::vector<storage::Flow> flows) override;

    void stop() override {};

private:
    std::size_t _mtu;
    UdpClient _client;
};

} // namespace p2np::out
