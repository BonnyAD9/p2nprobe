#include <iostream>

#include "args.hpp"

namespace p2np {

int start(int argc, char **argv);

} // namespace p2np

int main(int argc, char **argv) {
    try {
        return p2np::start(argc, argv);
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

namespace p2np {

int start(int argc, char **argv) {
    p2np::Args args({argv, static_cast<std::size_t>(argc)});

    std::cout
        << "pcap file path  : " << args.pcapFilePath() << '\n'
        << "host address    : " << args.hostAddress() << '\n'
        << "host port       : " << args.hostPort() << '\n'
        << "active timeout  : " << args.activeTimeout() << '\n'
        << "inactive timeout: " << args.inactiveTimeout() << std::endl;

    return EXIT_SUCCESS;
}

} // namespace p2np
