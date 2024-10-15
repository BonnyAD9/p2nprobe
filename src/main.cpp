#include <array>
#include <iostream>

#include "args.hpp"
#include "pcap.hpp"
#include "pcap_src.hpp"
#include "pipeline.hpp"
#include "text_exporter.hpp"

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
    const p2np::Args args({ argv, static_cast<std::size_t>(argc) });

    pcap::init();

    Pipeline pipeline(
        { args.pcap_file_path() },
        std::unique_ptr<Exporter>(new TextExporter()),
        std::chrono::duration_cast<Duration>(args.active_timeout()),
        std::chrono::duration_cast<Duration>(args.inactive_timeout())
    );

    pipeline.run();

    return EXIT_SUCCESS;
}

} // namespace p2np
