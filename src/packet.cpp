#include "packet.hpp"

#include "parser/parsers.hpp"

namespace p2np {

bool Packet::parse() { return parsers::parse(*this); }

} // namespace p2np
