#include "args.hpp"

#include <stdexcept>

namespace p2np {

using std::string_literals::operator""s;

template<typename D, typename S>
void set_unset_value(D &dst, S src, const char * argName);

void assert_set(const std::string &str, const char *argName);

template<typename I>
char *next(I &iterator, const I &end);

Args::Args(std::span<char *> args) {
    // skip the path to executable
    args = args.subspan(1);

    // Parse the arguments
    auto end = args.end();
    for (auto arg = args.begin(); arg != end; ++arg) {
        if (*arg == "-a"s) {
            set_unset_value(
                m_activeTimeout,
                std::atoll(next(arg, end)),
                "Active timeout (-a)"
            );
        } else if (*arg == "-i"s) {
            set_unset_value(
                m_inactiveTimeout,
                std::atoll(next(arg, end)),
                "Inactive timeout (-i)"
            );
        } else if (m_hostPort) {
            if (!m_pcapFilePath.empty()) {
                throw std::runtime_error("Unknown argument `"s + *arg + "`.");
            }
            m_pcapFilePath = *arg;
        } else {
            parseAddress(*arg);
        }
    }

    // Validate the arguments
    assert_set(m_pcapFilePath, "pcap file path");
    assert_set(m_hostAddress, "host adress and port");
}

void Args::parseAddress(std::string value) {
    auto idx = static_cast<std::size_t>(std::ranges::find(value, ':') - value.begin());
    if (idx == value.size()) {
        if (m_pcapFilePath.empty()) {
            // Host address and pcap file are in different order
            m_pcapFilePath = std::move(value);
            return;
        }
        throw std::runtime_error("Missing port number in the host address.");
    }

    // Split the address and port

    long long r;
    try {
        r = std::stoll(value.substr(idx + 1));
    } catch (...) {
        throw std::runtime_error("Failed to parse port number.");
    }
    if (r > std::numeric_limits<std::uint16_t>::max()) {
        throw std::runtime_error("Port number is outside of valid range.");
    }
    set_unset_value(m_hostPort, r, "host");

    m_hostAddress = value.substr(0, idx);
}

template<typename D, typename S>
void set_unset_value(D& dst, S src, const char *argName) {
    if (dst) {
        throw std::runtime_error(argName + " is set multiple times."s);
    }
    dst = D(std::move(src));
}

void assert_set(const std::string &str, const char *argName) {
    if (str.empty()) {
        throw std::runtime_error("missing "s + argName + ".");
    }
}

template<typename I>
char *next(I &iterator, const I &end) {
    // This expects char pointer iterator.
    static_assert(std::is_same_v<
        std::remove_reference_t<decltype(*iterator)>,
        char *
    >);

    // Get the current value in case of error message.
    char *value = *iterator++;
    if (iterator == end) {
        throw std::runtime_error(
            "Expected another argument after `"s + value + "`."
        );
    }
    return *iterator;
}

} // namespace p2np

