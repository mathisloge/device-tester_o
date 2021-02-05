#pragma once
#include <nmea/nmea_messages.hpp>
namespace protocol
{
    class NmeaHandler final
    {
    public:
        void handle(const NmeaMessage &msg);
        void handle(const NmeaUnsupported &msg);
        void handle(const NmeaDTM &msg);
        void handle(const NmeaGLL &msg);
    };
} // namespace protocol
