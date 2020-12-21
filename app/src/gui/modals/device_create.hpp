#pragma once
#include <boost/asio.hpp>
#include <future>
#include "base_modal.hpp"

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
        DeviceCreate();

    private:
        void drawContent() override;
        void drawDeviceInterfaceSerial();
        void drawDeviceInterfaceTcp();
        void drawDeviceInterfaceUdp();
        void clearInputs();
        bool addDevice();
        void checkConnection();

    private:
        std::string input_name_;
        DeviceInterface device_interface_;

        struct SerialInput
        {
            static constexpr int kSizePort = 150;
            char port[kSizePort];
            int baud_rate;
            boost::asio::serial_port_base::parity::type parity;
            int char_size;
            boost::asio::serial_port_base::flow_control::type flow_control;
            boost::asio::serial_port_base::stop_bits::type stop_bits;
        } serial_input_;

        std::future<std::pair<bool, std::string>> connection_check_fut_;
        bool has_valid_connection_;
        std::string error_msg_;
    };
} // namespace gui
