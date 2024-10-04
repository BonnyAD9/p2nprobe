#pragma once

#include <cstdint>

namespace p2np::parsers {

enum class IpType : std::uint8_t {
    HOP_BY_HOP_HEADER = 0,
    TCP = 0x06,
    ROUTING_HEADER = 46,
    FRAGMENT_HEADER = 44,
    ESP_HEADER = 50,
    AUTH_HEADER = 51,
    DST_OPTS_HEADER = 60,
    MOBILITY_HEADER = 135,
    HOST_ID_HEADER = 139,
    SHIM6_HEADER = 140,
};

} // namespace p2np::parsers
