#pragma once

#include <bit>
#include <cstdint>

namespace p2np {

static inline std::uint16_t endian_swap(std::uint16_t v) {
    return (v >> 8) | (v << 8);
}

static inline std::uint32_t endian_swap(std::uint32_t v) {
    constexpr std::uint32_t MASK = 0xff00ff00;
    v = ((v & MASK) >> 8) | ((v >> 8) & MASK);
    return (v >> 16) | (v << 16);
}

template<typename T>
concept endian_swappable = requires(T i) {
    { endian_swap(i) } -> std::same_as<T>;
};

template<endian_swappable Int> inline Int from_be(Int v) {
    if constexpr (std::endian::native == std::endian::little) {
        return endian_swap(v);
    } else {
        return v;
    }
}

template<endian_swappable Int> inline Int to_be(Int v) { return from_be(v); }

template<endian_swappable Int> class __attribute__((packed)) Be {
public:
    Be(Int num) : _num(to_be(num)) { }

    operator Int() const { return from_be(_num); }

private:
    Int _num;
};

} // namespace p2np
