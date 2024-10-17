/// @author Jakub Antonín Štigler (xstigl00)

#include <iostream>

namespace p2np::cli {

void help() {
    std::cout
        << "Welcome to p2nprobe help by xstigl00\n"
           "\n"
           "Usage:\n"
           "  p2nprobe <host>:<port> <pcap-file> [flags]\n"
           "    Read packets from pcap file and export flow data to host.\n"
           "\n"
           "  p2nprobe -h\n"
           "    Show this help.\n"
           "\n"
           "Flags:\n"
           "  -h  -?  --help\n"
           "    Show this help.\n"
           "\n"
           "  -a <seconds>\n"
           "    Set the active timeout in seconds.\n"
           "\n"
           "  -i <seconds>\n"
           "    Set the inactive timeout in seconds.\n"
           "\n";
}

} // namespace p2np::cli
