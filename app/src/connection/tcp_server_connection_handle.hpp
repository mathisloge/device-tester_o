#pragma once
#include <memory>
#include "connection_handle.hpp"
namespace connection
{
    class TcpServerClient;
    class TcpServerConnectionHandle : public ConnectionHandle
    {
    public:
        virtual void processData(const std::shared_ptr<TcpServerClient> &client, std::span<uint8_t> data);
    };

} // namespace connection
