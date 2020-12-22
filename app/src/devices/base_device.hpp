#pragma once
#include "../connection/connection.hpp"

class BaseDevice
{
public:
    virtual void processData(std::span<uint8_t> data) = 0;
};
