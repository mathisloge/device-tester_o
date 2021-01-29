#pragma once
#include <cstdint>
#include <span>

namespace connection
{
    class ConnectionHandle
    {
    public:
        virtual void processData(std::span<uint8_t> data) = 0;
    };
} // namespace connection
