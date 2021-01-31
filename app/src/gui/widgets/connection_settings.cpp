#include <array>
#include <string_view>
#include <imgui.h>
#include "connection_settings.hpp"
#include "inputs.hpp"

namespace gui
{
    void drawSerialOptions(connection::SerialOptions &opts)
    {
        static constexpr std::array<std::string_view, 3> kNamesParity{"none", "odd", "even"};
        static constexpr std::array<std::string_view, 3> kNamesStopBits{"1", "1.5", "2"};
        static constexpr std::array<std::string_view, 3> kNamesFlowControl{"none", "software", "hardware"};

        ImGui::PushID(&opts);
        SimpleInputText("Port", &opts.port);
        int baud_rate = opts.baud_rate;
        if (ImGui::InputInt("Baud rate", &baud_rate, 100, 1000))
        {
            opts.baud_rate = baud_rate;
        }
        int char_size = opts.char_size.value();
        if (ImGui::InputInt("Character size", &char_size, 1, 2))
        {
            if (char_size >= 5 && char_size <= 8)
                opts.char_size = boost::asio::serial_port_base::character_size(char_size);
        }

        const int parity = static_cast<int>(opts.parity.value());
        if (ImGui::BeginCombo("Parity", (parity >= 0 && parity < 3) ? kNamesParity[parity].data() : "unknown"))
        {
            for (int i = 0; i < kNamesParity.size(); i++)
            {
                if (ImGui::Selectable(kNamesParity[i].data(), parity == i))
                    opts.parity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::type(i));
            }
            ImGui::EndCombo();
        }
        const int stop_bits = static_cast<int>(opts.stop_bits.value());
        if (ImGui::BeginCombo("Stop bits", (stop_bits >= 0 && stop_bits < 3) ? kNamesStopBits[stop_bits].data() : "unknown"))
        {
            for (int i = 0; i < kNamesStopBits.size(); i++)
            {
                if (ImGui::Selectable(kNamesStopBits[i].data(), stop_bits == i))
                    opts.stop_bits = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::type(i));
            }
            ImGui::EndCombo();
        }
        const int flow_control = static_cast<int>(opts.flow_control.value());
        if (ImGui::BeginCombo("Flow control", (flow_control >= 0 && flow_control < 3) ? kNamesFlowControl[flow_control].data() : "unknown"))
        {
            for (int i = 0; i < kNamesFlowControl.size(); i++)
            {
                if (ImGui::Selectable(kNamesFlowControl[i].data(), flow_control == i))
                    opts.flow_control = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::type(i));
            }
            ImGui::EndCombo();
        }
        ImGui::PopID();
    }

    void drawTcpOptions(connection::TcpOptions &opts)
    {
        ImGui::PushID(&opts);
        SimpleInputText("Address", &opts.server);
        int port = opts.server_port;
        if (ImGui::InputInt("Port", &port))
        {
            if (port >= 0 && port <= 65535)
                opts.server_port = port;
        }
        SimpleInputText("Service", &opts.service);
        ImGui::PopID();
    }

    void drawUdpOptions(connection::UdpOptions &opts)
    {
        ImGui::PushID(&opts);
        SimpleInputText("Write address", &opts.write_address);
        int wport = opts.write_port;
        if (ImGui::InputInt("Write Port", &wport))
        {
            if (wport >= 0 && wport <= 65535)
                opts.write_port = wport;
        }

        int rport = opts.listen_port;
        if (ImGui::InputInt("Listen Port", &rport))
        {
            if (rport >= 0 && rport <= 65535)
                opts.listen_port = rport;
        }

        static constexpr std::array<std::string_view, 3> kNamesProtocol{"none", "ipv4", "ipv6"};
        if (ImGui::BeginCombo("Listen protocol",
                              (opts.listen_protocol >= connection::UdpOptions::Protocol::none && opts.listen_protocol <= connection::UdpOptions::Protocol::ipv6)
                                  ? kNamesProtocol[static_cast<int>(opts.listen_protocol)].data()
                                  : "unknown"))
        {
            for (int i = 0; i < kNamesProtocol.size(); i++)
            {

                if (ImGui::Selectable(kNamesProtocol[i].data(), opts.listen_protocol == connection::UdpOptions::Protocol(i)))
                    opts.listen_protocol = connection::UdpOptions::Protocol(i);
            }
            ImGui::EndCombo();
        }
        ImGui::PopID();
    }

} // namespace gui
