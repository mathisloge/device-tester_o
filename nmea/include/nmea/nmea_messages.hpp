#pragma once
#include <cstdint>
#include <string>
enum class Talker
{
    GP, //! GPS, SBAS, QZSS
    GL, //! GLONASS
    GA, //! Galileo
    GB, //! BeiDou
    GN  //! any combination of GNSS
};

enum class DatumRef
{
    wgs84,
    pz90,
    user_defined
};
enum class DirectionIndicator
{
    north,
    south,
    east,
    west
};

enum class PositionFixStatus
{
    unknown,
    no_fix,
    dead_reckoning,
    gnss_fix,
    differential_fix,
    rtk_float,
    rtk_fixed
};
//! \todo convert to chrono time
struct Time
{
    uint32_t hours;
    uint32_t minutes;
    uint32_t seconds;
    uint32_t milliseconds;
};

struct NmeaMessage
{
    Talker talker;
};

struct NmeaUnsupported : NmeaMessage
{
    std::string message;
};

/**
 * @brief This message gives the difference between the current datum and the reference datum.
 */
struct NmeaDTM : NmeaMessage
{
    DatumRef datum;
    double offset_latitude;
    DirectionIndicator latitude_indicator; //! only north / south
    double offset_longitude;
    DirectionIndicator longitude_indicator; //! only east / west
    double offset_altitude;
    DatumRef reference_datum;
};

/**
 * @brief Latitude and longitude, with time of position fix and status
 * @attention The output of this message is dependent on the currently selected datum @see NmeaDTM
 */
struct NmeaGLL : NmeaMessage
{
    double latitude;
    double longitude;
    Time time;
    bool data_valid;
    PositionFixStatus position_fix;
};

namespace nmea::detail
{
    struct NmeaLatLonRep
    {
        uint32_t degrees;
        double minutes;
        DirectionIndicator indicator;
        operator double() const
        {
            const double trans = degrees + (minutes / 60.);
            if (indicator == DirectionIndicator::south ||
                indicator == DirectionIndicator::west)
                return -1. * trans;
            else
                return trans;
        }
    };
} // namespace nmea::detail
