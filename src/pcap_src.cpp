#include "pcap_src.hpp"

#include <chrono>
#include <iostream>

namespace p2np {

std::chrono::system_clock::time_point timeval_to_chrono(timeval t);

PcapSrc::PcapSrc(const std::string &pcap_file)
    : _packet(new Packet()), _pcap(pcap::open_offline(pcap_file.c_str())) { }

NextCode PcapSrc::next() noexcept {
    using namespace std::chrono;

    pcap_pkthdr *header;
    const u_char *data;
    auto r = pcap_next_ex(_pcap.get(), &header, &data);

    switch (r) {
    case 1:
        _packet->data = { reinterpret_cast<const char *>(data), header->len };
        _packet->timestamp = timeval_to_chrono(header->ts);
        return NextCode::SUCCESS;
    case 0:
        return NextCode::TIMEOUT;
    case PCAP_ERROR_BREAK:
        return NextCode::END;
    case PCAP_ERROR_NOT_ACTIVATED:
        std::cerr << "pcap_next_ex() has failed: Pcap not activated.\n";
        return NextCode::ERROR;
    case PCAP_ERROR:
        std::cerr << "pcap_next_ex() has failed: " << pcap_geterr(_pcap.get())
                  << '\n';
        return NextCode::ERROR;
    default:
        std::cerr << "pcap_next_ex() has returned invalid value.\n";
        return NextCode::ERROR;
    }
}

std::chrono::system_clock::time_point timeval_to_chrono(timeval t) {
    using namespace std::chrono;
    return system_clock::time_point(duration_cast<system_clock::duration>(
        seconds(t.tv_sec) + microseconds(t.tv_usec)
    ));
}

} // namespace p2np
