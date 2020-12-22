#pragma once
#include "connection_win.hpp"
#include "../../connection/serial_connection.hpp"
namespace gui
{
    class SerialConnectionWin : public ConnectionWin
    {
    public:
        explicit SerialConnectionWin(const std::shared_ptr<SerialConnection> &serial_connection);

    private:
        void drawConnectionOverview() override;
        void drawConnectionDetails() override;
        void drawConnectionSettings() override;

    private:
        std::shared_ptr<SerialConnection> serial_connection_;
    };
} // namespace gui
