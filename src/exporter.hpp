#pragma once

#include <vector>

#include "flow.hpp"

namespace p2np {

/// @brief Exporter for flows.
class Exporter {
public:
    /// @brief Start the exporter.
    virtual void start() = 0;

    /// @brief Export the given flows.
    /// @param flows
    virtual void send(std::vector<Flow> flows) = 0;

    /// @brief Stop the exporter.
    virtual void stop() = 0;

    virtual ~Exporter() = default;
};

} // namespace p2np
