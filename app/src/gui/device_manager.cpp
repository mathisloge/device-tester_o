#include <thread>
#include <future>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include "device_manager.hpp"
#include "../connection/serial_connection.hpp"
namespace gui
{

    class DumbConnectionHandle : public ConnectionHandle
    {
    public:
        std::string &buffer_;
        DumbConnectionHandle(std::string &buffer) : buffer_{buffer} {}
        void processData(std::span<uint8_t> data) override { buffer_.append(data.begin(), data.end()); }
    };

    DeviceManager::DeviceManager()
        : should_stop_{false}, io_thread_{std::bind(&DeviceManager::ioThread, this)}
    {
    }

    void DeviceManager::ioThread()
    {
        while (!should_stop_)
        {
            io_context_.run();
        }
    }

    std::future<std::tuple<bool, std::string, std::string>> DeviceManager::testSerialConnection(const std::string &devname,
                                                                                                unsigned int baud_rate,
                                                                                                boost::asio::serial_port_base::parity::type parity,
                                                                                                int char_size,
                                                                                                boost::asio::serial_port_base::flow_control::type flow_ctrl,
                                                                                                boost::asio::serial_port_base::stop_bits::type stop_bits)
    {
        return std::async([=]() {
            std::string tmp_buffer;
            tmp_buffer.reserve(1000);
            DumbConnectionHandle con_handle{tmp_buffer};
            auto con = std::make_shared<SerialConnection>(con_handle, io_context_);
            try
            {
                spdlog::debug("test connection for serial device at {}@{}", devname, baud_rate);
                con->setOptions(devname, baud_rate, boost::asio::serial_port_base::parity(parity),
                                boost::asio::serial_port_base::character_size(char_size),
                                boost::asio::serial_port_base::flow_control(flow_ctrl),
                                boost::asio::serial_port_base::stop_bits(stop_bits));
            }
            catch (const std::exception &err)
            {
                SPDLOG_ERROR("while connecting to serial device {}. with error: {}", devname, err.what());
                return std::make_tuple(false, std::string(err.what()), tmp_buffer);
            }
            if (!con->isConnected())
            {
                return std::make_tuple(false, fmt::format("Could set options but couldn't connect to {}@{}", devname, baud_rate), tmp_buffer);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            con->disconnect();
            return std::make_tuple(true, fmt::format("Connected to {}@{}", devname, baud_rate), tmp_buffer);
        });
    }

    DeviceManager::~DeviceManager()
    {
        should_stop_ = true;
        if (io_thread_.joinable())
        {
            io_thread_.join();
        }
    }
} // namespace gui
