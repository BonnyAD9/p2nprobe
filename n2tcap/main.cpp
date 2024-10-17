/// @author Jakub Antonín Štigler (xstigl00)

#include <iostream>

#include "record_printer.hpp"
#include "udp_listener.hpp"

using namespace n2tc;

int start(std::span<char *> args);

int main(int argc, char **argv) {
    try {
        return start({ argv, std::size_t(argc) });
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    } catch (...) {
        std::cerr << "Error: Lol you wish." << '\n';
    }
}

int start(std::span<char *> args) {
    args = args.subspan(1);
    if (args.size() != 2) {
        throw std::runtime_error("Invalid number of arguments");
    }

    auto address = args[0];
    auto port = std::uint16_t(std::atol(args[1]));

    UdpListener listener(address, port);

    while (true) {
        print(NetFlowV5Records(listener.recv()));
    }
}
