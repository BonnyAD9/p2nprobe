#pragma once

#include <cstddef>
#include <memory>

#include "exporter.hpp"
#include "flow_cache.hpp"
#include "packet.hpp"
#include "pcap_src.hpp"

namespace p2np {

/// @brief Pipeline for parsing packets.
class Pipeline {
public:
    /// @brief Create new pipeline.
    /// @param src Packet source.
    /// @param exporter Exporter for flows.
    /// @param active_timeout Active timeout for flows.
    /// @param inactive_timeout Inactive timeout for flows.
    Pipeline(
        PcapSrc src,
        std::unique_ptr<Exporter> exporter,
        Duration active_timeout,
        Duration inactive_timeout
    )
        : _flows(active_timeout, inactive_timeout),
          _src(std::move(src)),
          _exporter(std::move(exporter)) { }

    /// @brief Run the pipeline.
    void run();

    /// @brief Gets the total numbe of packets that were processed in this
    /// pipeline.
    [[nodiscard]]
    std::size_t total_packets() const {
        return _total_packets;
    }

    /// @brief Gets the total size of all packets processed in this pipeline.
    [[nodiscard]]
    std::size_t total_size() const {
        return _total_size;
    }

private:
    void process(Packet &pkt);

    std::size_t _total_packets = 0;
    std::size_t _total_size = 0;
    FlowCache _flows;
    PcapSrc _src;

    std::unique_ptr<Exporter> _exporter;
};

} // namespace p2np
