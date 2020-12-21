#include "device_create.hpp"
#include "../imgui_commons.hpp"
#include <string_view>
#include <array>
#include <fmt/format.h>

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

    bool DeviceCreate::addDevice()
    {
        return true;
    }

    void DeviceCreate::checkConnection()
    {
        switch (device_interface_)
        {
        case DeviceInterface::serial:
            connection_check_.in_progress = true;
            connection_check_.progress = device_manager_.testSerialConnection(serial_input_.port,
                                                                              serial_input_.baud_rate,
                                                                              serial_input_.parity,
                                                                              serial_input_.char_size,
                                                                              serial_input_.flow_control,
                                                                              serial_input_.stop_bits);
            break;
        default:
            error_msg_ = "Device not supported";
        }
    }

    void DeviceCreate::drawContent()
    {
        ImGui::InputTextWithHint("Name", "personal identifier. no effect", input_name_.data(), input_name_.size());

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
            drawDeviceInterfaceSerial();
            break;
        case DeviceInterface::tcp:
            drawDeviceInterfaceTcp();
            break;
        case DeviceInterface::udp:
            drawDeviceInterfaceUdp();
            break;
        default:
            ImGui::Text("unknown device interface");
        }

        const bool has_connection_check = testConnectionInProg();

        ImGui::Separator();
        if (!error_msg_.empty())
        {
            static const ImVec4 err_col{1.f, 0.f, 0.f, 1.f};
            ImGui::TextColored(err_col, error_msg_.c_str());
        }
        if (!success_msg_.empty())
        {
            static const ImVec4 succs_col{0.1f, 0.8f, 0.1f, 1.f};
            ImGui::TextColored(succs_col, success_msg_.c_str());
        }
        std::string loader_str{"Test connection"};
        if (has_connection_check)
        {
            loader_str = fmt::format("Loading {}", "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
        }
        if (Button(loader_str.c_str(), has_connection_check))
        {
            checkConnection();
        }
        if(std::get<bool>(connection_check_.result)) {
            ImGui::Text("Data output: ");
            ImGui::TextWrapped(std::get<2>(connection_check_.result).c_str());
        }
        ImGui::Separator();

        ImGui::SetItemDefaultFocus();
        if (Button("Cancel", has_connection_check))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (Button("Add connection", has_connection_check))
        {
            if (addDevice())
            {
                ImGui::CloseCurrentPopup();
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

    void DeviceCreate::drawDeviceInterfaceSerial()
    {
        static constexpr std::array<std::string_view, 3> kNamesParity{"none", "odd", "even"};
        static constexpr std::array<std::string_view, 3> kNamesStopBits{"1", "1.5", "2"};
        static constexpr std::array<std::string_view, 3> kNamesFlowControl{"none", "software", "hardware"};

        ImGui::InputTextWithHint("Port", "", serial_input_.port.data(), serial_input_.port.size());
        ImGui::InputInt("Baud rate", &serial_input_.baud_rate, 100, 1000);
        ImGui::InputInt("Character size", &serial_input_.char_size, 1, 2);

        if (ImGui::BeginCombo("Parity", (serial_input_.parity >= 0 && serial_input_.parity < 3) ? kNamesParity[serial_input_.parity].data() : "unknown"))
        {
            for (int i = 0; i < kNamesParity.size(); i++)
            {
                ImGui::PushID(this);
                if (ImGui::Selectable(kNamesParity[i].data(), serial_input_.parity == i))
                    serial_input_.parity = boost::asio::serial_port_base::parity::type(i);
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Stop bits", (serial_input_.stop_bits >= 0 && serial_input_.stop_bits < 3) ? kNamesStopBits[serial_input_.stop_bits].data() : "unknown"))
        {
            for (int i = 0; i < kNamesStopBits.size(); i++)
            {
                ImGui::PushID(this);
                if (ImGui::Selectable(kNamesStopBits[i].data(), serial_input_.stop_bits == i))
                    serial_input_.stop_bits = boost::asio::serial_port_base::stop_bits::type(i);
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Flow control", (serial_input_.flow_control >= 0 && serial_input_.flow_control < 3) ? kNamesFlowControl[serial_input_.flow_control].data() : "unknown"))
        {
            for (int i = 0; i < kNamesFlowControl.size(); i++)
            {
                ImGui::PushID(this);
                if (ImGui::Selectable(kNamesFlowControl[i].data(), serial_input_.flow_control == i))
                    serial_input_.flow_control = boost::asio::serial_port_base::flow_control::type(i);
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
    }
    void DeviceCreate::drawDeviceInterfaceTcp()
    {

        ImGui::Text("NOT IMPLEMENTED");
    }
    void DeviceCreate::drawDeviceInterfaceUdp()
    {

        ImGui::Text("NOT IMPLEMENTED");
    }

    void DeviceCreate::clearInputs()
    {
        device_interface_ = DeviceInterface::serial;
        input_name_ = "";
        input_name_.resize(150);

        serial_input_.port = "";
        serial_input_.port.resize(SerialInput::kSizePort);

        serial_input_.baud_rate = 9600;
        serial_input_.char_size = SerialConnection::kDefaultCharSize;
        serial_input_.parity = SerialConnection::kDefaultPar;
        serial_input_.flow_control = SerialConnection::kDefaultFlow;
        serial_input_.stop_bits = SerialConnection::kDefaultStopBits;
    }
} // namespace gui
