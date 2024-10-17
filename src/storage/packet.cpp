/// @author Jakub Antonín Štigler (xstigl00)

#include "packet.hpp"

#include "../parser/parsers.hpp"

namespace p2np::storage {

bool Packet::parse() { return parsers::parse(*this); }

} // namespace p2np::storage
