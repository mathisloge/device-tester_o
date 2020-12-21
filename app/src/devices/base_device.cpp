#include "base_device.hpp"

boost::signals2::connection BaseDevice::connectData(const DataSig::slot_type &sub)
{
    return data_sig_.connect(sub);
}
void BaseDevice::processData(std::span<uint8_t> data)
{
    data_sig_(data);
}
