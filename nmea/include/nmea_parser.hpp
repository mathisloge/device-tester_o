#pragma once
#include <vector>
#include "nmea_parser_helper.hpp"

class NmeaParser
{
public:
    template <typename Handler>
    bool parseOneWithDispatch(std::vector<uint8_t>::const_iterator &begin, std::vector<uint8_t>::const_iterator end, Handler &handler)
    {
        nmea::detail::AllNmeaMessages msg_var;
        const bool success = nmea::detail::parse_nmea(begin, end, msg_var);
        if (success)
            boost::apply_visitor([&handler](const auto &msg) { handler.handle(msg); }, msg_var);
        return success;
    }
};