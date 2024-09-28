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
    std::size_t totalPackets() const { return m_totalPackets; }
    /// @brief Gets the total size of all packets processed in this pipeline.
    std::size_t totalSize() const { return m_totalSize; }

private:
    std::size_t m_totalPackets = 0;
    std::size_t m_totalSize = 0;
};

} // namespace p2np
