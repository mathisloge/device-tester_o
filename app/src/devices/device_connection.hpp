#pragma once
#include <cstdint>
#include <span>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include "base_device.hpp"
#include "../connection/connection_handle.hpp"

class DeviceConnection : public connection::ConnectionHandle
{
public:
    using DataSig = boost::signals2::signal<void(std::span<uint8_t>)>;

public:
    boost::signals2::connection connectData(const DataSig::slot_type &subscriber);
    boost::signals2::connection addDevice(boost::shared_ptr<BaseDevice> device);
    virtual void processData(std::span<uint8_t> data) override;

private:
    DataSig data_sig_;
};
