#include "protocol_nmea.hpp"
#include <nmea/nmea_parser.hpp>
#include <spdlog/spdlog.h>
ProtocolNmea::ProtocolNmea(NmeaHandler &handler)
    : handler_{handler}
{
}

const char *ProtocolNmea::name() const
{
    return "NMEA0183";
}

std::pair<ProtoCIter, ProtoCIter> ProtocolNmea::consumeOneMessage(ProtoCIter begin, ProtoCIter end)
{
    nmea::detail::AllNmeaMessages msg_var;
    ProtoCIter read_end = begin;
    if (nmea::parseOneWithDispatch(read_end, end, handler_))
        return std::make_pair(begin, read_end);
    return std::make_pair(begin, begin);
}
