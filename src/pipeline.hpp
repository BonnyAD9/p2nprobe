#pragma once

#include <cstddef>

#include "packet.hpp"

namespace p2np {

/// @brief Pipeline for parsing packets.
class Pipeline {
public:
    Pipeline() = default;

    /// @brief Process the next packet.
    /// @param pkt Packet to process.
    void process(Packet &pkt);

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
    std::size_t _total_packets = 0;
    std::size_t _total_size = 0;
};

} // namespace p2np
