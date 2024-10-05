#include "args.hpp"

#include <iterator>
#include <stdexcept>

namespace p2np {

using std::string_literals::operator""s;

template<typename I, typename Item>
concept r_iterator = std::input_iterator<I> && requires(I i) {
    requires std::is_same_v<std::remove_reference_t<decltype(*i)>, Item>;
};

template<typename D, typename S>
void set_unset_value(D &dst, S src, const char *arg_name);

void assert_set(const std::string &str, const char *arg_name);

template<r_iterator<char *> I> char *next(I &iterator, const I &end);

Args::Args(std::span<char *> args) {
    // skip the path to executable
    args = args.subspan(1);

    // Parse the arguments
    auto end = args.end();
    for (auto arg = args.begin(); arg != end; ++arg) {
        if (*arg == "-a"s) {
            set_unset_value(
                _active_timeout,
                std::atoll(next(arg, end)),
                "Active timeout (-a)"
            );
        } else if (*arg == "-i"s) {
            set_unset_value(
                _inactive_timeout,
                std::atoll(next(arg, end)),
                "Inactive timeout (-i)"
            );
        } else if (_host_port) {
            if (!_pcap_file_path.empty()) {
                throw std::runtime_error("Unknown argument `"s + *arg + "`.");
            }
            _pcap_file_path = *arg;
        } else {
            parse_address(*arg);
        }
    }

    // Validate the arguments
    assert_set(_pcap_file_path, "pcap file path");
    assert_set(_host_address, "host adress and port");
}

void Args::parse_address(std::string value) {
    auto idx = static_cast<std::size_t>(
        std::ranges::find(value, ':') - value.begin()
    );
    if (idx == value.size()) {
        if (_pcap_file_path.empty()) {
            // Host address and pcap file are in different order
            _pcap_file_path = std::move(value);
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
    set_unset_value(_host_port, r, "host");

    _host_address = value.substr(0, idx);
}

template<typename D, typename S>
void set_unset_value(D &dst, S src, const char *arg_name) {
    if (dst) {
        throw std::runtime_error(arg_name + " is set multiple times."s);
    }
    dst = D(std::move(src));
}

void assert_set(const std::string &str, const char *arg_name) {
    if (str.empty()) {
        throw std::runtime_error("missing "s + arg_name + ".");
    }
}

template<r_iterator<char *> I> char *next(I &iterator, const I &end) {
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
