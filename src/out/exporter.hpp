/// @author Jakub Antonín Štigler (xstigl00)

#pragma once

#include <vector>

#include "../storage/flow.hpp"

namespace p2np::out {

/// @brief Exporter for flows.
class Exporter {
public:
    /// @brief Start the exporter.
    virtual void start() = 0;

    /// @brief Export the given flows.
    /// @param flows
    virtual void send(std::vector<storage::Flow> flows) = 0;

    /// @brief Stop the exporter.
    virtual void stop() = 0;

    virtual ~Exporter() = default;
};

} // namespace p2np::out
