#pragma once

namespace p2np::pcap {

/// @brief Get last pcap error message.
/// @return Last pcap error message.
const char *strerror();

/// @brief Initialize the pcap library.
void init();

} // namespace p2np::pcap
