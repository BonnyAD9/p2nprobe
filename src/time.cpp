#include "time.hpp"

namespace p2np {

static Instant sys_up_instant;

void init_uptime() { sys_up_instant = Clock::now(); }

Instant sys_up() { return sys_up_instant; }

Duration sys_uptime() { return Clock::now() - sys_up_instant; }

} // namespace p2np
