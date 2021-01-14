#pragma once
#include <boost/asio.hpp>
#include <future>
#include "base_modal.hpp"

#include "../device_manager.hpp"
#include "../../connection/serial_connection.hpp"
namespace gui
{
    class DeviceCreate : public BaseModal
    {
    public:
        enum class DeviceInterface
        {
            serial,
            tcp,
            udp,
            count
        };

    public:
        explicit DeviceCreate(DeviceManager &device_manager);

    private:
        void drawContent() override;
        void drawDeviceInterfaceSerial();
        void drawDeviceInterfaceTcp();
        void drawDeviceInterfaceUdp();
        bool testConnectionInProg();
        void clearInputs();
        bool addConnection();
        void checkConnection();

    private:
        DeviceManager &device_manager_;

        std::string input_name_;

        DeviceInterface device_interface_;

        struct SerialInput
        {
            static constexpr int kSizePort = 150;
            std::string port;
            int baud_rate;
            boost::asio::serial_port_base::parity::type parity;
            int char_size;
            boost::asio::serial_port_base::flow_control::type flow_control;
            boost::asio::serial_port_base::stop_bits::type stop_bits;
        } serial_input_;

        struct TcpInput {
            std::string address;
            int port;
            std::string service;
            char packet_end;
        } tcp_input_;

        struct
        {
            bool in_progress;
            std::future<std::tuple<bool, std::string, std::string>> progress;
            std::tuple<bool, std::string, std::string> result;
        } connection_check_;
        std::string error_msg_;
        std::string success_msg_;
    };
} // namespace gui
