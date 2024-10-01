#pragma once

#include "../packet.hpp"

namespace p2np::parsers {

/// @brief Parses ehternet packet.
/// @param pkt Packet with the data to parse and to fill.
/// @return True if the packet is TCP, otherwise false.
bool ethernet(Packet &pkt);

/// @brief Parses ipv4 packet.
/// @param pkt Packet with data to parse and to fill.
/// @return True if the packet is TCP, therwise false.
bool ipv4(Packet &pkt);

/// @brief Parses ipv6 packet.
/// @param pkt Packet with data to parse and to fill.
/// @return True if the packet is TCP, otherwise false.
bool ipv6(Packet &pkt);

/// @brief Parse tcp packet.
/// @param pkt Packet with data to parse and to fill.
/// @return True if necesary data wa successfully readed, otherwise false.
bool tcp(Packet &pkt);

} // namespace parsers
