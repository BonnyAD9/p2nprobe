#pragma once

#include <span>

#include "../packet.hpp"

namespace p2np::parsers {

/// @brief Parse the given packet.
/// @param pkt Packet to parse
/// @return True if the packet is ipv4 TCP, otherwise false.
inline bool parse(Packet &pkt);

/// @brief Parses ehternet packet.
/// @param pkt Packet with the data to parse and to fill.
/// @return True if the packet is ipv4 TCP, otherwise false.
bool ethernet(Packet &pkt, std::span<const char> data);

/// @brief Parses ipv4 packet.
/// @param pkt Packet with data to parse and to fill.
/// @return True if the packet is ipv4 TCP, therwise false.
bool ipv4(Packet &pkt, std::span<const char> data);

/// @brief Parse tcp packet.
/// @param pkt Packet with data to parse and to fill.
/// @return True if necesary data wa successfully readed, otherwise false.
bool tcp(Packet &pkt, std::span<const char> data);

inline bool parse(Packet &pkt) { return ethernet(pkt, pkt.data); }

} // namespace p2np::parsers
