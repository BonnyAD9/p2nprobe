/// @author Jakub Antonín Štigler (xstigl00)

#include <array>
#include <iostream>

#include "args.hpp"
#include "out/net_flow_v5.hpp"
#include "pcap/pcap.hpp"
#include "pcap/source.hpp"
#include "pipeline.hpp"

namespace p2np {

int start(int argc, char **argv);

} // namespace p2np

int main(int argc, char **argv) {
    try {
        return p2np::start(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}

namespace p2np {

int start(int argc, char **argv) {
    const p2np::Args args({ argv, std::size_t(argc) });

    pcap::init();
    init_uptime();

    UdpClient client(args.host_address(), args.host_port());

    Pipeline pipeline(
        { args.pcap_file_path() },
        std::unique_ptr<out::Exporter>(new out::NetFlowV5(std::move(client))),
        std::chrono::duration_cast<Duration>(args.active_timeout()),
        std::chrono::duration_cast<Duration>(args.inactive_timeout())
    );

    pipeline.run();

    return EXIT_SUCCESS;
}

} // namespace p2np
