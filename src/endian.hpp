/// @author Jakub Antonín Štigler (xstigl00)

#pragma once

#include <bit>
#include <cstdint>

namespace p2np {

/// @brief Swap the byte order for the given value.
static inline std::uint16_t endian_swap(std::uint16_t v) {
    return (v >> 8) | (v << 8);
}

/// @brief Swap the byte order for the given value.
static inline std::uint32_t endian_swap(std::uint32_t v) {
    constexpr std::uint32_t MASK = 0xff00ff00;
    v = ((v & MASK) >> 8) | ((v << 8) & MASK);
    return (v >> 16) | (v << 16);
}

/// @brief Swap the byte order for the given value.
static inline std::int32_t endian_swap(std::int32_t v) {
    return std::int32_t(endian_swap(std::uint32_t(v)));
}

/// Type for which the function `endian_swap` is defined.
template<typename T>
concept endian_swappable = requires(T i) {
    { endian_swap(i) } -> std::same_as<T>;
};

/// @brief Convert value from big endian to host byte order.
template<endian_swappable Int> inline Int from_be(Int v) {
    if constexpr (std::endian::native == std::endian::little) {
        return endian_swap(v);
    } else {
        return v;
    }
}

/// @brief Convert value from host order to big endian.
template<endian_swappable Int> inline Int to_be(Int v) { return from_be(v); }

/// @brief Wraps value that is stored in big endian byte order.
template<endian_swappable Int> class __attribute__((packed)) Be {
public:
    Be() = default;
    /// @brief Create big endian value.
    Be(Int num) : _num(to_be(num)) { }

    operator Int() const { return from_be(_num); }

    Be<Int> &operator+=(const Be<Int> &other) {
        *this = *this + other;
        return *this;
    }

private:
    Int _num;
};

} // namespace p2np
