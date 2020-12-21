#pragma once
#include <cstdint>
#include <span>
class ConnectionHandle
{
public:
    virtual void processData(std::span<uint8_t> data) = 0;
};
