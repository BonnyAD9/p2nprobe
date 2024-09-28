#pragma once

#include <memory>
#include <string>

#include "pcap.hpp"
#include "packet.hpp"

namespace p2np {

/// @brief Result from calling the method `PcapSrc::next()`
enum class NextCode {
    /// @brief Successfully readed next packet. It is available with
    /// `PcapSrc::packet()`
    SUCCESS,
    /// @brief Timed out. Only when reading live.
    TIMEOUT,
    /// @brief All packet from file are readed.
    END,
    /// @brief Some error occured while reading the next packet. Error is
    /// logged to stderr.
    ERROR,
};

/// @brief Wrapper around `pcap_t`
class PcapSrc {
public:
    /// @brief Open pcap file.
    /// @param pcapFile Path to pcap file.
    PcapSrc(const std::string &pcapFile);

    /// @brief Gets the next packet. The data is valid only until the next call
    /// to `next()`.
    /// @return The next packet. Valid only until the next call to `next()`.
    NextCode next() noexcept;

    /// @brief Gets the last packet retrieved with `next()`. The returned value
    /// is valid only if the last call to `next()` has returned
    /// `NextCode::SUCCESS` and only until the next call to `next()`.
    ///
    /// Errors are logged to stderr.
    /// @return The last packet retrieved with `next()`.
    Packet &packet() { return *m_packet; }

private:
    std::unique_ptr<Packet> m_packet;
    pcap::UniquePcapT m_pcap;
};

} // namespace p2np
