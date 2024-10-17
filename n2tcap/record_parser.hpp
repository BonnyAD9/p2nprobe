/// @author Jakub Antonín Štigler (xstigl00)

#pragma once

#include "../src/out/net_flow_v5.hpp"

namespace n2tc {

using namespace p2np;
using namespace p2np::out;

class NetFlowV5Records {
public:
    NetFlowV5Records(std::span<const char> data);

    [[nodiscard]]
    const NetFlowV5Header &header() const {
        return *_header;
    }

    [[nodiscard]]
    std::span<const NetFlowV5Record> records() const {
        return _records;
    }

private:
    const NetFlowV5Header *_header;
    std::span<const NetFlowV5Record> _records;
};

} // namespace n2tc
