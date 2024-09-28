#include <iostream>
#include <array>

#include "pcap.hpp"
#include "args.hpp"
#include "pcap_src.hpp"
#include "pipeline.hpp"

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

    pcap::init();
    PcapSrc src(args.pcapFilePath());

    Pipeline pipeline;

    for (auto code = src.next(); code != NextCode::END; code = src.next()) {
        if (code != NextCode::SUCCESS) {
            continue;
        }
        pipeline.process(src.packet());
    }

    std::cout
        << "Total packets: " << pipeline.totalPackets() << '\n'
        << "Total size   : " << pipeline.totalSize() << '\n'
        << "Avg size     : "
            << (pipeline.totalSize() / pipeline.totalPackets()) << std::endl;

    return EXIT_SUCCESS;
}

} // namespace p2np
