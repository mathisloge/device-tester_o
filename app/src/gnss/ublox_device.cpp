#include "ublox_device.hpp"

UbloxDevice::UbloxDevice(UbloxHandler &ublox_handler, NmeaHandler &nmea_handler)
    : dispatcher_{ProtocolUblox{ublox_handler}, ProtocolNmea{nmea_handler}}
{
}
void UbloxDevice::processData(const uint8_t *data, const size_t len)
{
    dispatcher_.appendData(data, len);
}
