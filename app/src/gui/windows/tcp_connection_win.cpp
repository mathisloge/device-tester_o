#include "tcp_connection_win.hpp"
#include <imgui.h>
#include "../widgets/inputs.hpp"
#include "../widgets/connection_settings.hpp"
namespace gui
{
    TcpConnectionWin::TcpConnectionWin(const std::shared_ptr<connection::Tcp> &connection, DeviceConnection &device_connection)
        : ConnectionWin(connection->identifier(), device_connection), connection_{connection}
    {
    }

    void TcpConnectionWin::drawConnectionOverview()
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

    void TcpConnectionWin::drawConnectionRawInput()
    {
        const bool is_connected = connection_->isConnected();
        if (Button("Send", !is_connected))
        {
        }
    }

    void TcpConnectionWin::drawConnectionDetails()
    {
    }
    void TcpConnectionWin::drawConnectionSettings()
    {
    }
} // namespace gui
