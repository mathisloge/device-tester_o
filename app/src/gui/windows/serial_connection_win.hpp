#pragma once
#include "connection_win.hpp"
#include "../../connection/serial_connection.hpp"

namespace gui
{
    class SerialConnectionWin : public ConnectionWin
    {
    public:
        explicit SerialConnectionWin(const std::shared_ptr<connection::Serial> &serial_connection, DeviceConnection &device_connection);

    private:
        void drawConnectionOverview() override;
        void drawConnectionRawInput() override;
        void drawConnectionDetails() override;
        void drawConnectionSettings() override;

    private:
        std::shared_ptr<connection::Serial> serial_connection_;
        connection::SerialOptions serial_options_;
        std::string tx_str_;
    };
} // namespace gui
