#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

namespace p2np {

class Args {
public:
    /// @brief Parse the arguments.
    /// @param args Arguments to parse.
    /// @throws when the arguments are invalid.
    Args(std::span<char *> args);

    /// @brief Get path to the pcap file.
    /// @return Path to the pcap file.
    [[nodiscard]]
    const std::string &pcap_file_path() const {
        return _pcap_file_path;
    }

    /// @brief Get the host address.
    /// @return Host address.
    [[nodiscard]]
    const std::string &host_address() const {
        return _host_address;
    }

    /// @brief Get the host port number.
    /// @return Port number of the host.
    [[nodiscard]]
    std::uint16_t host_port() const {
        return _host_port;
    }

    /// @brief Get the active timeout.
    /// @return Active timeout.
    [[nodiscard]]
    std::chrono::seconds active_timeout() const {
        return _active_timeout.value_or(std::chrono::seconds(DEFAULT_TIMEOUT));
    }

    /// @brief Get the inactive timeout.
    /// @return Inactive timeout.
    [[nodiscard]]
    std::chrono::seconds inactive_timeout() const {
        return _inactive_timeout.value_or(std::chrono::seconds(DEFAULT_TIMEOUT)
        );
    }

    static constexpr std::size_t DEFAULT_TIMEOUT = 60;

private:
    void parse_address(std::string value);

    std::string _pcap_file_path;

    std::string _host_address;
    std::uint16_t _host_port = 0;

    std::optional<std::chrono::seconds> _active_timeout;
    std::optional<std::chrono::seconds> _inactive_timeout;
};

} // namespace p2np
