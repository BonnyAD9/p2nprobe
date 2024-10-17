/// @author Jakub Antonín Štigler (xstigl00)

#pragma once

#include "record_parser.hpp"

namespace n2tc {

void print(const NetFlowV5Records &l);

void print(const NetFlowV5Header &h);

void print(const NetFlowV5Record &r, Instant time);

} // namespace n2tc
