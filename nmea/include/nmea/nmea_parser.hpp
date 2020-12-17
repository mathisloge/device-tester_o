#pragma once
#include <vector>
#include "nmea_parser_helper.hpp"

namespace nmea
{
    template <typename Iterator, typename Handler>
    bool parseOneWithDispatch(Iterator &begin, Iterator end, Handler &handler)
    {
        nmea::detail::AllNmeaMessages msg_var;
        while (begin != end && (*begin) != '$')
            begin++;
        if (begin == end)
            return false;
            
        const bool success = nmea::detail::parse_nmea(begin, end, msg_var);
        if (success)
            boost::apply_visitor([&handler](const auto &msg) { handler.handle(msg); }, msg_var);
        return success;
    }
} // namespace nmea