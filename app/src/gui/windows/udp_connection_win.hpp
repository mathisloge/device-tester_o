#pragma once
#include "connection_win.hpp"
#include "../../connection/udp_connection.hpp"

namespace gui
{
    class UdpConnectionWin : public ConnectionWin
    {
    public:
        explicit UdpConnectionWin(const std::shared_ptr<connection::Udp> &connection, DeviceConnection &device_connection);

    private:
        void drawConnectionOverview() override;
        void drawConnectionRawInput() override;
        void drawConnectionDetails() override;
        void drawConnectionSettings() override;

    private:
        std::shared_ptr<connection::Udp> connection_;
    };
} // namespace gui
