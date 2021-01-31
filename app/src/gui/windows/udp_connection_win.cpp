#include <span>
#include "udp_connection_win.hpp"
#include <imgui.h>
#include "../widgets/inputs.hpp"
#include "../widgets/connection_settings.hpp"
namespace gui
{
    UdpConnectionWin::UdpConnectionWin(const std::shared_ptr<connection::Udp> &connection, DeviceConnection &device_connection)
        : ConnectionWin(connection->identifier(), device_connection),
          connection_{connection},
          options_{connection->udpOptions()}
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
        SimpleInputText("Write", &udp_write_);
        ImGui::Checkbox("Newline", &append_new_line_);
        if (ImGui::Button("Send"))
        {
            udp_write_.append("\n");
            if (udp_write_.size() > 0)
                connection_->write(std::span<uint8_t>{reinterpret_cast<uint8_t *>(udp_write_.data()), udp_write_.size()});
            udp_write_ = "";
        }
    }

    void UdpConnectionWin::drawConnectionDetails()
    {
    }
    void UdpConnectionWin::drawConnectionSettings()
    {
        drawUdpOptions(connection_->udpOptions());
        if (ImGui::Button("Reset"))
        {
            options_ = connection_->udpOptions();
        }
        if (ImGui::Button("Apply options"))
        {
            connection_->setOptions(options_);
            try
            {
                connection_->applyOptions();
            }
            catch (const std::exception &ex)
            {
                error_str_ = ex.what();
            }
        }
    }
} // namespace gui
