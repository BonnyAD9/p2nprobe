#include "flow_cache.hpp"

namespace p2np {

void FlowCache::add(const Packet &pkt) {
    _cache[pkt].add(pkt);
    /// TODO: timeouts
}

} // namespace p2np
