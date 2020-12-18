#pragma once
#include <cstdint>
class ConnectionHandle
{
public:
    virtual void processData(const uint8_t *data, const size_t len) = 0;
};
