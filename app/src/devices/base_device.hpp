#pragma once
#include <boost/signals2.hpp>
#include "../connection/connection.hpp"

class BaseDevice : public ConnectionHandle
{
public:
    using DataSig = boost::signals2::signal<void(std::span<uint8_t>)>;

public:
    boost::signals2::connection connectData(const DataSig::slot_type &sub);
    virtual void processData(std::span<uint8_t> data) override;

private:
    DataSig data_sig_;
};
