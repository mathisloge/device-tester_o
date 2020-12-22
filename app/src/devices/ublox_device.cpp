#include "ublox_device.hpp"

UbloxDevice::UbloxDevice(UbloxHandler &ublox_handler, NmeaHandler &nmea_handler)
    : dispatcher_{ProtocolUblox{ublox_handler}, ProtocolNmea{nmea_handler}}
{
}
void UbloxDevice::processData(std::span<uint8_t> data)
{
    dispatcher_.appendData(std::forward<std::span<uint8_t>>(std::move(data)));
}
