#include "udp_connection_win.hpp"
#include "../imgui_commons.hpp"
#include <imgui.h>
namespace gui
{
    UdpConnectionWin::UdpConnectionWin(const std::shared_ptr<connection::Udp> &connection, DeviceConnection &device_connection)
        : ConnectionWin(connection->identifier(), device_connection), connection_{connection}
    {
    }

    void UdpConnectionWin::drawConnectionOverview()
    {
        const bool is_connected = connection_->isConnected();
        if (is_connected)
        {
            if (ImGui::Button("Disconnect"))
                connection_->disconnect();
        }
        else
        {
            if (ImGui::Button("Connect"))
                connection_->connect();
        }
    }

    void UdpConnectionWin::drawConnectionRawInput()
    {
        const bool is_connected = connection_->isConnected();
        if (Button("Send", !is_connected))
        {
        }
    }

    void UdpConnectionWin::drawConnectionDetails()
    {
    }
    void UdpConnectionWin::drawConnectionSettings()
    {
    }
} // namespace gui
