#pragma once
#include "nmea_messages.hpp"
class NmeaHandler
{
public:
    virtual void handle(const NmeaMessage &msg) = 0;
};