#pragma once

#include "exporter.hpp"

namespace p2np::out {

/// @brief Exporter that prints to stdout.
class Text : public Exporter {
public:
    void start() override { }

    void send(std::vector<storage::Flow> flows) override;

    void stop() override { }

private:
    static void send(const storage::Flow &flow);
};

} // namespace p2np::out
