#include "flow_cache.hpp"

#include "time.hpp"

namespace p2np {

void FlowCache::add(const Packet &pkt) {
    auto &flow = _cache[pkt];
    if (!flow) {
        auto new_flow = std::make_unique<Flow>(pkt);
        flow = new_flow.get();
        _flows.emplace(pkt, std::move(new_flow));
        return;
    }

    if (pkt.timestamp - flow->first() > _active_timeout ||
        pkt.timestamp - flow->last() > _inactive_timeout) {
        _export_q.push_back(*flow);
        *flow = Flow(pkt);
        return;
    }

    flow->add(pkt);
}

std::vector<Flow> FlowCache::exported(Instant now) {
    std::vector<Flow> res;
    std::swap(res, _export_q);
    while (!_flows.empty() &&
           now - _flows.front().second->first() > _active_timeout) {
        auto [key, flow] = std::move(_flows.front());
        _flows.pop();
        _cache.erase(key);
        res.push_back(*flow);
    }
    return res;
}

} // namespace p2np
