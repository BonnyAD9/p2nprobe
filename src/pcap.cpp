#include "pcap.hpp"

#include <array>
#include <stdexcept>

#include <pcap/pcap.h>

namespace p2np::pcap {

using std::string_literals::operator""s;

static std::array<char, PCAP_ERRBUF_SIZE> errbuf;

const char *strerror() {
    return errbuf.data();
}

void init() {
    if (pcap_init(0, errbuf.data()) == PCAP_ERROR) {
        throw std::runtime_error("pcap_init() has failed: "s + strerror());
    }
}

} // namespace p2np::pcap
