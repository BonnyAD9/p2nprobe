#include <array>
#include <iostream>

#include "args.hpp"
#include "pcap.hpp"
#include "pcap_src.hpp"
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
    const p2np::Args args({ argv, static_cast<std::size_t>(argc) });

    pcap::init();
    PcapSrc src(args.pcap_file_path());

    Pipeline pipeline;

    for (auto code = src.next(); code != NextCode::END; code = src.next()) {
        if (code != NextCode::SUCCESS) {
            continue;
        }
        pipeline.process(src.packet());
    }

    std::cout << "Total packets: " << pipeline.total_packets() << '\n'
              << "Total size   : " << pipeline.total_size() << '\n'
              << "Avg size     : "
              << (pipeline.total_size() / pipeline.total_packets()) << '\n';

    return EXIT_SUCCESS;
}

} // namespace p2np
