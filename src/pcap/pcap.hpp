#pragma once

#include <memory>

#include <pcap/pcap.h>

namespace p2np::pcap {

namespace destroy {

/// @brief Deallocator for `pcap_t`
class PcapT {
public:
    void operator()(void *ptr) { pcap_close(reinterpret_cast<pcap_t *>(ptr)); }
};

} // namespace destroy

using UniquePcapT = std::unique_ptr<pcap_t, destroy::PcapT>;

/// @brief Get last pcap error message.
/// @return Last pcap error message.
const char *strerror();

/// @brief Initialize the pcap library.
/// @throws On errors.
void init();

/// @brief Open pcap file.
/// @param file Path to the pcap file.
/// @return Initialized pcap struct.
/// @throws When fails to open the pcap file.
UniquePcapT open_offline(const char *file);

} // namespace p2np::pcap
