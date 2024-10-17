/// @author Jakub Antonín Štigler (xstigl00)

#pragma once

#include <chrono>

namespace p2np {

using Clock = std::chrono::system_clock;
using Duration = Clock::duration;
using Instant = Clock::time_point;

/// @brief Stores the system uptime.
void init_uptime();

/// @brief Gets the stored system uptime.
Instant sys_up();

/// @brief Gets the duration since the system uptime.
Duration sys_uptime();

} // namespace p2np
