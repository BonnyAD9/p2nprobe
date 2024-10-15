#pragma once

#include <unistd.h>

namespace p2np {

class UniqueFd {
public:
    UniqueFd(const UniqueFd &) = delete;
    UniqueFd &operator=(const UniqueFd &) = delete;

    UniqueFd() = default;
    bool operator==(const UniqueFd &) const = default;
    bool operator!=(const UniqueFd &) const = default;
    UniqueFd &operator=(UniqueFd &&other) = default;

    UniqueFd(UniqueFd &&other) noexcept : _fd(other.release()) { }

    UniqueFd(int fd) : _fd(fd) { }

    /// @brief Check if the file descriptor is valid.
    explicit operator bool() const { return _fd == -1; }

    /// @brief Get the underlaying file descriptor.
    /// @return The underlaying file descriptor.
    [[nodiscard]]
    int get() const {
        return _fd;
    }

    int release() noexcept {
        const int res = _fd;
        _fd = -1;
        return res;
    }

    ~UniqueFd() {
        if (*this) {
            close(_fd);
            _fd = -1;
        }
    }

private:
    int _fd = -1;
};

} // namespace p2np