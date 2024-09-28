#include "pcap_src.hpp"

#include <iostream>

namespace p2np {

PcapSrc::PcapSrc(const std::string &pcapFile) :
    m_pcap(pcap::open_offline(pcapFile.c_str())),
    m_packet(new Packet())
{}

NextCode PcapSrc::next() noexcept {
    pcap_pkthdr *header;
    const u_char *data;
    auto r = pcap_next_ex(m_pcap.get(), &header, &data);

    switch (r) {
        case 1:
            m_packet->data = {
                reinterpret_cast<const char *>(data),
                header->len
            };
            m_packet->timestamp = header->ts;
            return NextCode::SUCCESS;
        case 0:
            return NextCode::TIMEOUT;
        case PCAP_ERROR_BREAK:
            return NextCode::END;
        case PCAP_ERROR_NOT_ACTIVATED:
            std::cerr << "pcap_next_ex() has failed: Pcap not activated.\n";
            return NextCode::ERROR;
        case PCAP_ERROR:
            std::cerr
                << "pcap_next_ex() has failed: "
                << pcap_geterr(m_pcap.get())
                << '\n';
            return NextCode::ERROR;
        default:
            std::cerr << "pcap_next_ex() has returned invalid value.\n";
            return NextCode::ERROR;
    }
}

} // namespace p2np

