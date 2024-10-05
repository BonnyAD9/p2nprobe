#include "parsers.hpp"
namespace p2np::parsers {

bool tcp(Packet &pkt, std::span<const char> data) {
    (void)pkt;
    (void)data;
    return true;
}

} // namespace p2np::parsers
