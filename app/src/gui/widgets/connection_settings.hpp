#pragma once

#include "../../connection/serial_connection.hpp"
#include "../../connection/tcp_connection.hpp"
#include "../../connection/udp_connection.hpp"

namespace gui
{
    void drawSerialOptions(connection::SerialOptions &opts);
    void drawTcpOptions(connection::TcpOptions& opts);
    void drawUdpOptions(connection::UdpOptions& opts);
} // namespace gui
