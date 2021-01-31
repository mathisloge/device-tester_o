#include "tcp_connection_win.hpp"
#include <imgui.h>
#include "../widgets/inputs.hpp"
#include "../widgets/connection_settings.hpp"
namespace gui
{
    TcpConnectionWin::TcpConnectionWin(const std::shared_ptr<connection::Tcp> &connection, DeviceConnection &device_connection)
        : ConnectionWin(connection->identifier(), device_connection),
          connection_{connection},
          options_{connection->tcpOptions()},
          append_new_line_{true}
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
        SimpleInputText("Write", &tx_buffer_);
        ImGui::Checkbox("Newline", &append_new_line_);
        if (Button("Send", !is_connected))
        {
            tx_buffer_.append("\n");
            if (tx_buffer_.size() > 0)
                connection_->write(std::span<uint8_t>{reinterpret_cast<uint8_t *>(tx_buffer_.data()), tx_buffer_.size()});
            tx_buffer_ = "";
        }
    }

    void TcpConnectionWin::drawConnectionDetails()
    {
    }
    void TcpConnectionWin::drawConnectionSettings()
    {
        drawTcpOptions(options_);
        if (ImGui::Button("Reset"))
        {
            options_ = connection_->tcpOptions();
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
