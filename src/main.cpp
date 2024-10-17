/// @author Jakub Antonín Štigler (xstigl00)

#include <array>
#include <iostream>

#include "cli/args.hpp"
#include "cli/help.hpp"
#include "out/net_flow_v5.hpp"
#include "pcap/pcap.hpp"
#include "pcap/source.hpp"
#include "pipeline.hpp"

namespace p2np {

void start(std::span<char *> a);
void probe(const cli::Args &args);

} // namespace p2np

int main(int argc, char **argv) {
    try {
        p2np::start({ argv, std::size_t(argc) });
        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "An error occured.\n";
        return EXIT_FAILURE;
    }
}

namespace p2np {

void start(std::span<char *> a) {
    const cli::Args args(a);

    switch (args.action()) {
    case cli::Action::Probe:
        probe(args);
        return;
    case cli::Action::Help:
        cli::help();
        return;
    }
}

void probe(const cli::Args &args) {
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
}

} // namespace p2np
