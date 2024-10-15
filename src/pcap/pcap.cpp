#include "pcap.hpp"

#include <array>
#include <stdexcept>

#include <pcap/pcap.h>

#include "../ip_address.hpp"

namespace p2np::pcap {

using std::string_literals::operator""s;

static std::array<char, PCAP_ERRBUF_SIZE> errbuf;

const char *strerror() { return errbuf.data(); }

void init() {
    if (pcap_init(0, errbuf.data()) == PCAP_ERROR) {
        throw std::runtime_error("pcap_init() has failed: "s + strerror());
    }
}

UniquePcapT open_offline(const char *file) {
    UniquePcapT res(pcap_open_offline(file, errbuf.data()));

    if (!res) {
        throw std::runtime_error(
            "pcap_open_offline(\""s + file + "\") has failed: " + strerror()
        );
    }

    return res;
}

} // namespace p2np::pcap
