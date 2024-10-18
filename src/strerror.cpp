/// @author Jakub Antonín Štigler (xstigl00)

#include "strerror.hpp"

#include <array>
#include <cstring>

namespace p2np {

std::string strerror() { return strerror(errno); }

std::string strerror(int err) {
    std::array<char, 256> buf;
    const char *msg = strerror_r(err, buf.data(), 256);
    return msg;
}

} // namespace p2np
