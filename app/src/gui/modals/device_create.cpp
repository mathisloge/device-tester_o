#include "device_create.hpp"
#include "../imgui_commons.hpp"
#include <string_view>
#include <array>
#include <fmt/format.h>
#include <imgui_stdlib.h>

static int InputTextCallback(ImGuiInputTextCallbackData *data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        std::string *str = (std::string *)data->UserData;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char *)str->c_str();
    }
    return 0;
}
bool SimpleInputText(const char *label, std::string *str, ImGuiInputTextFlags flags = 0)
{
    //flags |= ImGuiInputTextFlags_CallbackResize;
    return ImGui::InputText(label, str, flags, InputTextCallback, (void *)str);
}

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
        static const ImVec4 err_col{1.f, 0.f, 0.f, 1.f};
        static const ImVec4 succs_col{0.1f, 0.8f, 0.1f, 1.f};
        SimpleInputText("Name", &input_name_);
        const bool has_input_name = !input_name_.empty();
        if (!has_input_name)
        {
            ImGui::TextColored(err_col, "Name cannot be empty");
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

        const bool conn_check_in_prog = testConnectionInProg();

        ImGui::Separator();
        if (!error_msg_.empty())
        {
            ImGui::TextColored(err_col, "Error:");
            ImGui::TextWrapped(error_msg_.c_str());
        }
        if (!success_msg_.empty())
        {
            ImGui::TextColored(succs_col, "Success:");
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

    void DeviceCreate::drawDeviceInterfaceSerial()
    {
        static constexpr std::array<std::string_view, 3> kNamesParity{"none", "odd", "even"};
        static constexpr std::array<std::string_view, 3> kNamesStopBits{"1", "1.5", "2"};
        static constexpr std::array<std::string_view, 3> kNamesFlowControl{"none", "software", "hardware"};

        SimpleInputText("Port", &serial_input_.port);
        int baud_rate = serial_input_.baud_rate;
        if (ImGui::InputInt("Baud rate", &baud_rate, 100, 1000))
        {
            serial_input_.baud_rate = baud_rate;
        }
        int char_size = serial_input_.char_size.value();
        if (ImGui::InputInt("Character size", &char_size, 1, 2))
        {
            serial_input_.char_size = boost::asio::serial_port_base::character_size(char_size);
        }

        const int parity = static_cast<int>(serial_input_.parity.value());
        if (ImGui::BeginCombo("Parity", (parity >= 0 && parity < 3) ? kNamesParity[parity].data() : "unknown"))
        {
            for (int i = 0; i < kNamesParity.size(); i++)
            {
                ImGui::PushID(this);
                if (ImGui::Selectable(kNamesParity[i].data(), parity == i))
                    serial_input_.parity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::type(i));
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
        const int stop_bits = static_cast<int>(serial_input_.stop_bits.value());
        if (ImGui::BeginCombo("Stop bits", (stop_bits >= 0 && stop_bits < 3) ? kNamesStopBits[stop_bits].data() : "unknown"))
        {
            for (int i = 0; i < kNamesStopBits.size(); i++)
            {
                ImGui::PushID(this);
                if (ImGui::Selectable(kNamesStopBits[i].data(), stop_bits == i))
                    serial_input_.stop_bits = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::type(i));
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
        const int flow_control = static_cast<int>(serial_input_.flow_control.value());
        if (ImGui::BeginCombo("Flow control", (flow_control >= 0 && flow_control < 3) ? kNamesFlowControl[flow_control].data() : "unknown"))
        {
            for (int i = 0; i < kNamesFlowControl.size(); i++)
            {
                ImGui::PushID(this);
                if (ImGui::Selectable(kNamesFlowControl[i].data(), flow_control == i))
                    serial_input_.flow_control = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::type(i));
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
    }
    void DeviceCreate::drawDeviceInterfaceTcp()
    {
        SimpleInputText("Address", &tcp_input_.server);
        int port = tcp_input_.server_port;
        if (ImGui::InputInt("Port", &port))
        {
            if (port >= 0 && port <= 65535)
                tcp_input_.server_port = port;
        }
        SimpleInputText("Service", &tcp_input_.service);
    }
    void DeviceCreate::drawDeviceInterfaceUdp()
    {
        SimpleInputText("Write address", &udp_input_.write_address);
        int wport = udp_input_.listen_port;
        if (ImGui::InputInt("Port", &wport))
        {
            if (wport >= 0 && wport <= 65535)
                udp_input_.listen_port = wport;
        }

        int rport = udp_input_.listen_port;
        if (ImGui::InputInt("Port", &rport))
        {
            if (rport >= 0 && rport <= 65535)
                udp_input_.listen_port = rport;
        }

        static constexpr std::array<std::string_view, 3> kNamesProtocol{"none", "ipv4", "ipv6"};
        if (ImGui::BeginCombo("Listen protocol",
                              (udp_input_.listen_protocol >= connection::UdpOptions::Protocol::none && udp_input_.listen_protocol <= connection::UdpOptions::Protocol::ipv6) ? kNamesProtocol[static_cast<int>(udp_input_.listen_protocol)].data() : "unknown"))
        {
            for (int i = 0; i < kNamesProtocol.size(); i++)
            {
                ImGui::PushID(this);
                if (ImGui::Selectable(kNamesProtocol[i].data(), udp_input_.listen_protocol == connection::UdpOptions::Protocol(i)))
                    udp_input_.listen_protocol = connection::UdpOptions::Protocol(i);
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
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
