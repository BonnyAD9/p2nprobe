#pragma once

#include <string>
#include <cstdint>
#include <optional>
#include <chrono>

namespace p2np {

class Args {
public:
    /// @brief Parse the arguments.
    /// @param args Arguments to parse.
    /// @throws when the arguments are invalid.
    Args(std::span<char *> args);

    /// @brief Get path to the pcap file.
    /// @return Path to the pcap file.
    const std::string &pcapFilePath() const { return m_pcapFilePath; }

    /// @brief Get the host address.
    /// @return Host address.
    const std::string &hostAddress() const { return m_hostAddress; }

    /// @brief Get the host port number.
    /// @return Port number of the host.
    std::uint16_t hostPort() const { return m_hostPort; }

    /// @brief Get the active timeout.
    /// @return Active timeout.
    std::chrono::seconds activeTimeout() const {
        return m_activeTimeout.value_or(std::chrono::seconds(60));
    }

    /// @brief Get the inactive timeout.
    /// @return Inactive timeout.
    std::chrono::seconds inactiveTimeout() const {
        return m_inactiveTimeout.value_or(std::chrono::seconds(60));
    }

private:
    void parseAddress(std::string value);

    std::string m_pcapFilePath;

    std::string m_hostAddress;
    std::uint16_t m_hostPort = 0;

    std::optional<std::chrono::seconds> m_activeTimeout;
    std::optional<std::chrono::seconds> m_inactiveTimeout;
};

} // namespace p2np
