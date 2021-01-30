#include "serial_connection_win.hpp"
#include <imgui.h>
#include "../widgets/inputs.hpp"
#include "../widgets/connection_settings.hpp"
namespace gui
{
    SerialConnectionWin::SerialConnectionWin(const std::shared_ptr<connection::Serial> &serial_connection, DeviceConnection &device_connection)
        : ConnectionWin(serial_connection->identifier(), device_connection), serial_connection_{serial_connection}
    {
    }

    void SerialConnectionWin::drawConnectionOverview()
    {
        ImGui::Text("Port: %s", serial_connection_->serialOptions().port.c_str());
        const bool is_connected = serial_connection_->isConnected();
        if (is_connected)
        {
            if (ImGui::Button("Disconnect"))
                serial_connection_->disconnect();
        }
        else
        {
            if (ImGui::Button("Connect"))
                serial_connection_->connect();
        }
    }

    void SerialConnectionWin::drawConnectionRawInput()
    {
        const bool is_connected = serial_connection_->isConnected();
        if (Button("Send", !is_connected))
        {
        }
    }

    void SerialConnectionWin::drawConnectionDetails()
    {
    }
    void SerialConnectionWin::drawConnectionSettings()
    {
    }
} // namespace gui
