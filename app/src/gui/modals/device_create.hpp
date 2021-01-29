#pragma once
#include <boost/asio.hpp>
#include <future>
#include "base_modal.hpp"

#include "../device_manager.hpp"
#include "../../connection/serial_connection.hpp"
#include "../../connection/udp_connection.hpp"
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
        void syncInputs();
        bool addConnection();
        void checkConnection();

    private:
        DeviceManager &device_manager_;

        std::string input_name_;

        DeviceInterface device_interface_;

        connection::SerialOptions serial_input_;
        connection::TcpOptions tcp_input_;
        connection::UdpOptions udp_input_;

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
