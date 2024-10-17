/// @author Jakub Antonín Štigler (xstigl00)

#pragma once

namespace p2np::cli {

/// @brief The main action of the program.
enum class Action {
    /// @brief Read pcap file and export.
    Probe,
    /// @brief Print help.
    Help,
};

} // namespace p2np::cli
