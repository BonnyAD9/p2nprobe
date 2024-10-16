#pragma once

#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "../time.hpp"
#include "flow.hpp"
#include "flow_key.hpp"

namespace p2np::storage {

/// @brief Cache for flows with timeouts.
class FlowCache {
public:
    explicit FlowCache(const FlowCache &) = default;

    /// @brief Construct new flow cache.
    /// @param active_timeout Active timeout for packets.
    /// @param inactive_timeout Inactive timeout for packets.
    FlowCache(Duration active_timeout, Duration inactive_timeout)
        : _inactive_timeout(inactive_timeout),
          _active_timeout(active_timeout) { }

    /// @brief Add data from packet to the cache.
    /// @param pkt Packet with data added to the cache.
    void add(const Packet &pkt);

    /// @brief Get all flows that are ready to be exported.
    /// @param now The current time, used for timeouts.
    /// @return All flows that are ready to be exported.
    std::vector<Flow> exported(Instant now);

    /// @brief Get and remove all flows from the cache.
    /// @return All flows in the cache.
    std::vector<Flow> drain();

private:
    void pop_flow();
    std::vector<Flow> move_queue();

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

} // namespace p2np::storage
