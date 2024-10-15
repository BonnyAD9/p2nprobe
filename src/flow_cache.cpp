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
    while (!_flows.empty() &&
           now - _flows.front().second->first() > _active_timeout) {
        pop_flow();
    }
    return move_queue();
}

void FlowCache::pop_flow() {
    auto [key, flow] = std::move(_flows.front());
    _flows.pop();
    _cache.erase(key);
    _export_q.push_back(*flow);
}

std::vector<Flow> FlowCache::move_queue() {
    std::vector<Flow> res;
    std::swap(res, _export_q);
    return res;
}

std::vector<Flow> FlowCache::drain() {
    while (!_flows.empty()) {
        pop_flow();
    }
    return move_queue();
}

} // namespace p2np
