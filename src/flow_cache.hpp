#pragma once

#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "flow.hpp"
#include "flow_key.hpp"
#include "time.hpp"

namespace p2np {

class FlowCache {
public:
    explicit FlowCache(const FlowCache &) = default;

    FlowCache(Duration active_timeout, Duration inactive_timeout)
        : _inactive_timeout(inactive_timeout),
          _active_timeout(active_timeout) { }

    void add(const Packet &pkt);

    std::vector<Flow> exported(Instant now);

private:
    std::unordered_map<FlowKey, Flow *> _cache;
    std::queue<std::pair<FlowKey, std::unique_ptr<Flow>>> _flows;
    std::vector<Flow> _export_q;
    /// @brief Duration since last packet after which the flow is considered
    /// inactive.
    Duration _inactive_timeout;
    /// @brief Duration since the first packet after which the flow is
    /// exported.
    Duration _active_timeout;
};

} // namespace p2np
