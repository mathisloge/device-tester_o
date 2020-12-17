#include "nmea_parser.hpp"

NmeaParser::NmeaParser(NmeaHandler &handler)
    : handler_{handler}
{
}