#pragma once
#include "base_device.hpp"
#include "../connection/connection_handle.hpp"
#include "../protocols/protocol_ublox.hpp"
#include "../protocols/protocol_nmea.hpp"
#include "../protocols/protocol_dispatcher.hpp"
class UbloxDevice : public BaseDevice
{
public:
    explicit UbloxDevice(UbloxHandler &ublox_handler, NmeaHandler &nmea_handler);
    void processData(std::span<uint8_t> data) override;
    
private:
    ProtocolDispatcher<ProtocolUblox, ProtocolNmea> dispatcher_;
};
