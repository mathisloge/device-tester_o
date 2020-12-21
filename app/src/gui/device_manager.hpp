#pragma once
#include <future>
#include <tuple>
#include <thread>
#include <boost/asio.hpp>

namespace gui
{
    class DeviceManager
    {
    public:
        DeviceManager();
        std::future<std::tuple<bool, std::string, std::string>> testSerialConnection(const std::string &devname,
                                                                       unsigned int baud_rate,
                                                                       boost::asio::serial_port_base::parity::type parity,
                                                                       int char_size,
                                                                       boost::asio::serial_port_base::flow_control::type flow_ctrl,
                                                                       boost::asio::serial_port_base::stop_bits::type stop_bits);
        ~DeviceManager();

    private:
        void ioThread();

    private:
        bool should_stop_;
        boost::asio::io_context io_context_;
        std::thread io_thread_;
    };
} // namespace gui
