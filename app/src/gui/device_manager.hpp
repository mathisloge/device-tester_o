#pragma once
#include <future>
#include <tuple>
#include <thread>
#include <vector>
#include <boost/asio.hpp>
#include "../connection/connection.hpp"
#include "../connection/serial_connection.hpp"
#include "../connection/tcp_connection.hpp"
#include "../connection/udp_connection.hpp"
#include "../devices/device_connection.hpp"
#include "windows/connection_win.hpp"
namespace gui
{
    class DumbConnectionHandle;
    class DeviceManager
    {
    public:
        using DeviceMap = std::map<std::string, std::unique_ptr<DeviceConnection>>;
        using ConnectionMap = std::map<std::string, std::shared_ptr<Connection>>;
        using WindowMap =  std::map<std::string, std::unique_ptr<ConnectionWin>>;
    public:
        DeviceManager();
        void draw();
        void drawMenu();
        std::future<std::tuple<bool, std::string, std::string>> testSerialConnection(const std::string &devname,
                                                                                     unsigned int baud_rate,
                                                                                     boost::asio::serial_port_base::parity::type parity,
                                                                                     int char_size,
                                                                                     boost::asio::serial_port_base::flow_control::type flow_ctrl,
                                                                                     boost::asio::serial_port_base::stop_bits::type stop_bits);
        std::pair<bool, std::string> addSerialConnection(const std::string &identifier,
                                                         const std::string &devname,
                                                         unsigned int baud_rate,
                                                         boost::asio::serial_port_base::parity::type parity,
                                                         int char_size,
                                                         boost::asio::serial_port_base::flow_control::type flow_ctrl,
                                                         boost::asio::serial_port_base::stop_bits::type stop_bits);

        std::future<std::tuple<bool, std::string, std::string>> testTcpConnection(const std::string &address,
                                                                                  unsigned short port,
                                                                                  const std::string &service,
                                                                                  const char packet_end);
        std::pair<bool, std::string> addTcpConnection(const std::string &identifier,
                                                      const std::string &address,
                                                      unsigned short port,
                                                      const std::string &service,
                                                      const char packet_end);
        std::pair<bool, std::string> addUdpConnection(const std::string &identifier,
                                                      const std::string &write_address, const unsigned short send_port,
                                                      const unsigned short listen_port, const UdpConnection::Protocol protocol);

        void closeConnection();
        ~DeviceManager();

    private:
        void ioThread();

    private:
        bool should_stop_;
        boost::asio::io_context io_context_;
        std::thread io_thread_;

        DeviceMap devices_;
        ConnectionMap connections_;
        WindowMap windows_;
        
        std::unique_ptr<DumbConnectionHandle> connection_test_handle_;
        std::string connection_test_buffer_;
    };
} // namespace gui
