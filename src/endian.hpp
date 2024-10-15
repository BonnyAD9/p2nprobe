#pragma once

#include <bit>
#include <cstdint>

namespace p2np {

static inline std::uint16_t endian_swap(std::uint16_t v) {
    return (v >> 8) | (v << 8);
}

template<std::integral Int> inline Int from_be(Int v) {
    if constexpr (std::endian::native == std::endian::little) {
        return endian_swap(v);
    } else {
        return v;
    }
}

template<std::integral Int> inline Int to_be(Int v) { return from_be(v); }

template<std::integral Int> class __attribute__((packed)) Be {
public:
    Be(Int num) : _num(to_be(num)) { }

    operator Int() const { return from_be(_num); }

private:
    Int _num;
};

} // namespace p2np
