#include "device_create.hpp"
#include <string_view>
#include <array>
#include <fmt/format.h>
#include "../imgui_commons.hpp"
#include "../widgets/inputs.hpp"
#include "../widgets/connection_settings.hpp"

namespace gui
{
    static constexpr bool validDeviceInterface(DeviceCreate::DeviceInterface dev)
    {
        return dev < DeviceCreate::DeviceInterface::count && dev >= DeviceCreate::DeviceInterface::serial;
    }
    DeviceCreate::DeviceCreate(DeviceManager &device_manager)
        : BaseModal("Device creation"),
          device_manager_{device_manager},
          device_interface_{DeviceInterface::serial}
    {
        connection_check_.in_progress = false;
        clearInputs();
    }

    bool DeviceCreate::addConnection()
    {
        success_msg_ = "";
        switch (device_interface_)
        {
        case DeviceInterface::serial:
        {
            const auto add_ret = device_manager_.addSerialConnection(serial_input_);
            if (add_ret.first)
                return true;
            error_msg_ = add_ret.second;
            break;
        }
        case DeviceInterface::tcp:
        {
            const auto add_ret = device_manager_.addTcpConnection(tcp_input_);
            if (add_ret.first)
                return true;
            error_msg_ = add_ret.second;
            break;
        }
        case DeviceInterface::udp:
        {
            const auto add_ret = device_manager_.addUdpConnection(udp_input_);
            if (add_ret.first)
                return true;
            error_msg_ = add_ret.second;
            break;
        }
        default:
            error_msg_ = "Connection not supported.";
        }
        return false;
    }

    void DeviceCreate::checkConnection()
    {
        switch (device_interface_)
        {
        case DeviceInterface::serial:
            connection_check_.in_progress = true;
            connection_check_.progress = device_manager_.testSerialConnection(serial_input_);
            break;
        case DeviceInterface::tcp:
            connection_check_.in_progress = true;
            connection_check_.progress = device_manager_.testTcpConnection(tcp_input_);
            break;
        case DeviceInterface::udp:
            error_msg_ = "Connection check not available for udp";
            break;
        default:
            error_msg_ = "Connection not supported";
        }
    }

    void DeviceCreate::drawContent()
    {
       
        SimpleInputText("Name", &input_name_);
        const bool has_input_name = !input_name_.empty();
        if (!has_input_name)
        {
            ImGui::TextColored(kErrorColor, "Name cannot be empty");
        }
        static constexpr std::array<std::string_view, static_cast<int>(DeviceInterface::count)> kPreviewValueDevNames{"Serial", "TCP", "UDP"};

        if (ImGui::BeginCombo("Interface", validDeviceInterface(device_interface_) ? kPreviewValueDevNames[static_cast<int>(device_interface_)].data() : "unknown"))
        {
            for (int i = 0; i < kPreviewValueDevNames.size(); i++)
            {
                ImGui::PushID(this);
                if (ImGui::Selectable(kPreviewValueDevNames[i].data(), validDeviceInterface(device_interface_) &&
                                                                           device_interface_ == static_cast<DeviceInterface>(i)))
                    device_interface_ = static_cast<DeviceInterface>(i);
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }

        switch (device_interface_)
        {
        case DeviceInterface::serial:
            drawSerialOptions(serial_input_);
            break;
        case DeviceInterface::tcp:
            drawTcpOptions(tcp_input_);
            break;
        case DeviceInterface::udp:
            drawUdpOptions(udp_input_);
            break;
        default:
            ImGui::Text("unknown device interface");
        }

        const bool conn_check_in_prog = testConnectionInProg();

        ImGui::Separator();
        if (!error_msg_.empty())
        {
            ImGui::TextColored(kErrorColor, "Error:");
            ImGui::TextWrapped(error_msg_.c_str());
        }
        if (!success_msg_.empty())
        {
            ImGui::TextColored(kSuccessColor, "Success:");
            ImGui::TextWrapped(success_msg_.c_str());
        }
        std::string loader_str{"Test connection"};
        if (conn_check_in_prog)
        {
            loader_str = fmt::format("Loading {}", "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
        }
        if (Button(loader_str.c_str(), conn_check_in_prog))
        {
            syncInputs();
            checkConnection();
        }
        if (std::get<bool>(connection_check_.result))
        {
            ImGui::Text("Data output: ");
            if (ImGui::BeginChild("##output_content", ImVec2{-1.f, 150.f}))
            {
                ImGui::TextUnformatted(std::get<2>(connection_check_.result).c_str());
            }
            ImGui::EndChild();
        }
        ImGui::Separator();

        ImGui::SetItemDefaultFocus();
        if (Button("Cancel", conn_check_in_prog))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (Button("Add connection", !has_input_name || conn_check_in_prog))
        {
            syncInputs();
            if (addConnection())
            {
                ImGui::CloseCurrentPopup();
                clearInputs();
            }
        }
    }

    bool DeviceCreate::testConnectionInProg()
    {
        const auto is_ready = [](const std::future<std::tuple<bool, std::string, std::string>> &f) {
            return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        };
        if (connection_check_.in_progress && connection_check_.progress.valid())
        {
            if (!is_ready(connection_check_.progress))
            {
                return true;
            }
            connection_check_.result = connection_check_.progress.get();
            if (!std::get<bool>(connection_check_.result))
            {
                error_msg_ = std::get<1>(connection_check_.result);
                success_msg_ = "";
            }
            else
            {
                error_msg_ = "";
                success_msg_ = std::get<1>(connection_check_.result);
            }
            connection_check_.in_progress = false;
        }
        else if (connection_check_.in_progress)
        {
            return true;
        }
        return false;
    }

    void DeviceCreate::clearInputs()
    {
        device_interface_ = DeviceInterface::serial;
        input_name_ = "";

        serial_input_ = connection::SerialOptions();
        tcp_input_ = connection::TcpOptions();
        udp_input_ = connection::UdpOptions();
    }

    void DeviceCreate::syncInputs()
    {
        serial_input_.identifier = input_name_;
        tcp_input_.identifier = input_name_;
        udp_input_.identifier = input_name_;
    }
} // namespace gui
