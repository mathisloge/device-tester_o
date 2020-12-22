#include <thread>
#include <future>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include "device_manager.hpp"
#include "../connection/serial_connection.hpp"
#include "windows/serial_connection_win.hpp"
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
        : should_stop_{false},
          io_thread_{std::bind(&DeviceManager::ioThread, this)},
          connection_test_handle_{std::make_unique<DumbConnectionHandle>(connection_test_buffer_)}
    {
        connection_test_buffer_.reserve(1000);
    }

    void DeviceManager::draw()
    {
        for (auto &con_win : connection_windows_)
        {
            con_win->draw();
        }
    }

    void DeviceManager::ioThread()
    {
        while (!should_stop_)
        {
            try
            {
                io_context_.run();
            }
            catch (const std::exception &ex)
            {
                SPDLOG_ERROR("error in io context run {}", ex.what());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
            connection_test_buffer_.clear();
            auto con = std::make_shared<SerialConnection>(*connection_test_handle_, "connection_test_serial", io_context_);

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
                return std::make_tuple(false, std::string(err.what()), connection_test_buffer_);
            }
            if (!con->isConnected())
            {
                return std::make_tuple(false, fmt::format("Could set options but couldn't connect to {}@{}", devname, baud_rate), connection_test_buffer_);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            con->disconnect();
            return std::make_tuple(true, fmt::format("Connected to {}@{}", devname, baud_rate), connection_test_buffer_);
        });
    }

    std::pair<bool, std::string> DeviceManager::addSerialConnection(const std::string &identifier,
                                                                    const std::string &devname,
                                                                    unsigned int baud_rate,
                                                                    boost::asio::serial_port_base::parity::type parity,
                                                                    int char_size,
                                                                    boost::asio::serial_port_base::flow_control::type flow_ctrl,
                                                                    boost::asio::serial_port_base::stop_bits::type stop_bits)
    {
        if (auto it = serial_connections_.find(devname); it != serial_connections_.end())
        {
            return std::make_pair(false, "serial connection port already exits. please close it first");
        }
        auto &dev_instance = serial_connections_[devname];
        dev_instance.first = std::make_unique<DeviceConnection>();
        auto con = std::make_shared<SerialConnection>(*dev_instance.first, identifier, io_context_);
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
            serial_connections_.erase(devname);
            return std::make_pair(false, fmt::format("while connecting to serial device {}. with error: {}", devname, err.what()));
        }
        con->connect();
        dev_instance.second = std::move(con);
        auto con_win = std::make_unique<SerialConnectionWin>(dev_instance.second, *dev_instance.first);
        connection_windows_.push_back(std::move(con_win));
        return std::make_pair(true, "Connection established");
    }

    const DeviceManager::SerialMap &DeviceManager::serial_connections() const
    {
        return serial_connections_;
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
