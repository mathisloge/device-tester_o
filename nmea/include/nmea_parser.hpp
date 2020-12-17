#pragma once
#include "nmea_handler.hpp"
class NmeaParser
{
public:
    explicit NmeaParser(NmeaHandler &handler);

private:
    NmeaHandler &handler_;
};