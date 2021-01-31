#include "tcp_server_connection_handle.hpp"

namespace connection
{
    void TcpServerConnectionHandle::processData(const std::shared_ptr<TcpServerClient> &, std::span<uint8_t> data)
    {
        ConnectionHandle::processData(data);
    }
} // namespace connection
