/// @author Jakub Antonín Štigler (xstigl00)

#include "record_parser.hpp"

namespace n2tc {

NetFlowV5Records::NetFlowV5Records(std::span<const char> data)
    : _header(reinterpret_cast<const NetFlowV5Header *>(data.data())),
      _records(
          reinterpret_cast<const NetFlowV5Record *>(
              data.data() + sizeof(NetFlowV5Header)
          ),
          _header->count
      ) { }

} // namespace n2tc
