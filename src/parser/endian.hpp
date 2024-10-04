#pragma once

#include <cstdint>
#include <bit>

namespace p2np::parsers {

inline std::uint16_t from_be(std::uint16_t v) {
    if constexpr(std::endian::native == std::endian::little) {
        return (v << 8) | (v >> 8);
    } else {
        return v;
    }
}

} // namespace p2np::parsers
