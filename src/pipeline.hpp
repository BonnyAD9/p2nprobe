/// @author Jakub Antonín Štigler (xstigl00)

#pragma once

#include <cstddef>
#include <memory>

#include "out/exporter.hpp"
#include "pcap/source.hpp"
#include "storage/flow_cache.hpp"
#include "storage/packet.hpp"

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
        pcap::Source src,
        std::unique_ptr<out::Exporter> exporter,
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
    void process(storage::Packet &pkt);

    std::size_t _total_packets = 0;
    std::size_t _total_size = 0;
    storage::FlowCache _flows;
    pcap::Source _src;

    std::unique_ptr<out::Exporter> _exporter;
};

} // namespace p2np
