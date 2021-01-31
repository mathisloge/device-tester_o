#include "serial_connection_win.hpp"
#include <imgui.h>
#include "../widgets/inputs.hpp"
#include "../widgets/connection_settings.hpp"
namespace gui
{
    SerialConnectionWin::SerialConnectionWin(const std::shared_ptr<connection::Serial> &serial_connection, DeviceConnection &device_connection)
        : ConnectionWin(serial_connection->identifier(), device_connection),
          serial_connection_{serial_connection},
          serial_options_{serial_connection->serialOptions()}
    {
    }

    void SerialConnectionWin::drawConnectionOverview()
    {
        ImGui::Text("Port: %s@%i", serial_connection_->serialOptions().port.c_str(), serial_connection_->serialOptions().baud_rate);
        const bool is_connected = serial_connection_->isConnected();
        if (is_connected)
        {
            if (ImGui::Button("Disconnect"))
                serial_connection_->disconnect();
        }
        else
        {
            if (ImGui::Button("Connect"))
            {
                try
                {
                    serial_connection_->connect();
                }
                catch (const std::exception &ex)
                {
                    error_str_ = ex.what();
                }
            }
        }
    }

    void SerialConnectionWin::drawConnectionRawInput()
    {
        const bool is_connected = serial_connection_->isConnected();
        SimpleInputText("Input", &tx_str_);
        if (Button("Send", !is_connected))
        {
            if (tx_str_.size() > 0)
                serial_connection_->write(std::span<uint8_t>{reinterpret_cast<uint8_t *>(tx_str_.data()), tx_str_.size()});
            tx_str_ = "";
        }
    }

    void SerialConnectionWin::drawConnectionDetails()
    {
    }
    void SerialConnectionWin::drawConnectionSettings()
    {
        drawSerialOptions(serial_options_);
        if (ImGui::Button("Reset"))
        {
            serial_options_ = serial_connection_->serialOptions();
        }
        if (ImGui::Button("Apply options"))
        {
            serial_connection_->setOptions(serial_options_);
            try
            {
                serial_connection_->applyOptions();
            }
            catch (const std::exception &ex)
            {
                error_str_ = ex.what();
            }
        }
    }
} // namespace gui
