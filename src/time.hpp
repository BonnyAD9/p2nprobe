#pragma once

#include <chrono>

namespace p2np {

using Clock = std::chrono::system_clock;
using Duration = Clock::duration;
using Instant = Clock::time_point;

} // namespace p2np
