#include "protocol_ublox.hpp"
#include <comms/util/detect.h>

ProtocolUblox::ProtocolUblox(detail::proto::UbloxHandler &ubx_handler)
    : ubx_handler_{ubx_handler}
{
}
const char *ProtocolUblox::name() const
{
    return "ublox";
}

std::pair<ProtoCIter, ProtoCIter> ProtocolUblox::consumeOneMessage(ProtoCIter begin, ProtoCIter end)
{
    if (!isActive())
        return std::make_pair(begin, begin);

    std::pair<ProtoCIter, ProtoCIter> read_range;
    const auto status = ubx_instance_.processSingle(begin, end, read_range, ubx_handler_);
    return status == comms::ErrorStatus::Success ? read_range : std::make_pair(begin, begin);
}