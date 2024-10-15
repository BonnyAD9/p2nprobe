#pragma once

#include "exporter.hpp"

namespace p2np {

/// @brief Exporter that prints to stdout.
class TextExporter : public Exporter {
public:
    void start() override { }

    void send(std::vector<Flow> flows) override;

    void stop() override { }

private:
    static void send(const Flow &flow);
};

} // namespace p2np
