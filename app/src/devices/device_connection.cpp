#include "device_connection.hpp"
#include <functional>

boost::signals2::connection DeviceConnection::connectData(const DataSig::slot_type &subscriber)
{
    return data_sig_.connect(subscriber);
}
boost::signals2::connection DeviceConnection::addDevice(boost::shared_ptr<BaseDevice> device)
{
    // this would fail. some deduction could not been resolved. => return data_sig_.connect(DataSig::slot_type(&BaseDevice::processData, device.get()).track(device));
    return connectData(DataSig::slot_type([device](std::span<uint8_t> data) { device->processData(data); }).track(device));
}

void DeviceConnection::processData(std::span<uint8_t> data)
{

    data_sig_(data);
}
