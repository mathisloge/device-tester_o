#include "device_create.hpp"
#include <string_view>
#include <array>

namespace gui
{
    static constexpr bool validDeviceInterface(DeviceCreate::DeviceInterface dev)
    {
        return dev < DeviceCreate::DeviceInterface::count && dev >= DeviceCreate::DeviceInterface::serial;
    }
    DeviceCreate::DeviceCreate()
        : BaseModal("Device creation"), device_interface_{DeviceInterface::serial}, has_valid_connection_{false}
    {
        clearInputs();
    }

    bool DeviceCreate::addDevice()
    {
        return true;
    }

    void DeviceCreate::checkConnection()
    {
        
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

        ImGui::Separator();
        if (!error_msg_.empty())
        {
            static const ImVec4 err_col{1.f, 0.f, 0.f, 1.f};
            ImGui::TextColored(err_col, error_msg_.c_str());
        }
        if (ImGui::Button("Test connection"))
        {
            checkConnection();
        }
        ImGui::Separator();

        ImGui::SetItemDefaultFocus();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Add connection"))
        {
            if (addDevice())
            {
                ImGui::CloseCurrentPopup();
            }
        }
    }

    void DeviceCreate::drawDeviceInterfaceSerial()
    {
        static constexpr std::array<std::string_view, 3> kNamesParity{"none", "odd", "even"};
        static constexpr std::array<std::string_view, 3> kNamesStopBits{"1", "1.5", "2"};
        static constexpr std::array<std::string_view, 3> kNamesFlowControl{"none", "software", "hardware"};

        ImGui::InputTextWithHint("Port", "", serial_input_.port, SerialInput::kSizePort);
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
        has_valid_connection_ = false;
        device_interface_ = DeviceInterface::serial;
        input_name_.clear();
        input_name_.resize(150);

        serial_input_.baud_rate = 9600;
        serial_input_.char_size = SerialConnection::kDefaultCharSize;
        serial_input_.parity = SerialConnection::kDefaultPar;
        serial_input_.flow_control = SerialConnection::kDefaultFlow;
        serial_input_.stop_bits = SerialConnection::kDefaultStopBits;
    }
} // namespace gui
