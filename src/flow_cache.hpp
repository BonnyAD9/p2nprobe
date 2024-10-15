#pragma once

#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "flow.hpp"
#include "flow_key.hpp"

namespace p2np {

class FlowCache {
public:
    FlowCache() = default;
    explicit FlowCache(const FlowCache &) = default;

    void add(const Packet &pkt);

    std::vector<Flow> exported(std::chrono::system_clock::time_point now);

private:
    std::unordered_map<FlowKey, Flow *> _cache;
    std::queue<std::pair<FlowKey, std::unique_ptr<Flow>>> _flows;
    std::vector<Flow> _export_q;
    /// @brief Duration since last packet after which the flow is considered
    /// inactive.
    std::chrono::system_clock::time_point::duration _inactive_timeout;
    /// @brief Duration since the first packet after which the flow is
    /// exported.
    std::chrono::system_clock::time_point::duration _active_timeout;
};

} // namespace p2np
