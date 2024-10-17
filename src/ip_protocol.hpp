/// @author Jakub Antonín Štigler (xstigl00)

#pragma once

#include <cstdint>

namespace p2np {

/// @brief IP protocol values.
enum class IpProtocol : std::uint8_t {
    TCP = 0x06,
};

} // namespace p2np
