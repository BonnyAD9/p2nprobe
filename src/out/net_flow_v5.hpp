/// @author Jakub Antonín Štigler (xstigl00)

#pragma once

#include <string>

#include <netdb.h>

#include "../endian.hpp"
#include "../udp_client.hpp"
#include "exporter.hpp"

namespace p2np::out {

/// @brief Header for NetFlow v5
struct __attribute__((packed)) NetFlowV5Header {
    Be<std::uint16_t> version;
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

/// @brief Record for NetFlow v5
struct __attribute__((packed)) NetFlowV5Record {
    NetFlowV5Record(const storage::Flow &flow);

    IpAddress srcaddr;
    IpAddress dstaddr;
    IpAddress nexthop;
    Be<std::uint16_t> input = 0;
    Be<std::uint16_t> output = 0;
    Be<std::uint32_t> d_pkts;
    Be<std::uint32_t> d_octets;
    Be<std::int32_t> first;
    Be<std::int32_t> last;
    Be<std::uint16_t> srcport;
    Be<std::uint16_t> dstport;
    std::uint8_t pad1 = 0;
    std::uint8_t tcp_flags;
    IpProtocol prot;
    std::uint8_t tos;
    Be<std::uint16_t> src_as = 0;
    Be<std::uint16_t> dst_as = 0;
    std::uint8_t src_mask = 0;
    std::uint8_t dst_mask = 0;
    std::uint16_t pad2 = 0;
};

/// @brief Exporter for NetFlow v5 via UDP.
class NetFlowV5 : public Exporter {
public:
    /// @brief The default mtu in bytes.
    static constexpr std::size_t DEFAULT_MTU = 1500 - 8 - 40;
    /// @brief The maximum mtu in bytes.
    static constexpr std::size_t MAX_MTU = 1463;
    /// @brief The NetFlow version.
    static constexpr std::uint16_t VERSION = 5;

    /// @brief Create new NetFlow exporter.
    /// @param client UDP client to send the data.
    /// @param mtu Mtu for the exporter. It is clamped to `MAX_MTU`
    NetFlowV5(UdpClient client, std::size_t mtu = DEFAULT_MTU);

    void start() override {};

    void send(std::vector<storage::Flow> flows) override;

    void stop() override;

private:
    void send(std::span<storage::Flow> flows);
    void push();

    std::vector<char> _data;
    std::span<NetFlowV5Record> _records;
    std::span<NetFlowV5Record>::iterator _cur;
    NetFlowV5Header *_header;
    UdpClient _client;
};

} // namespace p2np::out
