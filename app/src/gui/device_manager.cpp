#include <thread>
#include <future>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include "params.hpp"
#include "device_manager.hpp"
#include "../connection/serial_connection.hpp"
#include "windows/serial_connection_win.hpp"
#include "windows/tcp_connection_win.hpp"
#include "windows/udp_connection_win.hpp"
#include "../config/recent.hpp"
namespace gui
{

    class DumbConnectionHandle : public connection::ConnectionHandle
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
        refreshRecents();
    }

    void DeviceManager::draw()
    {
        for (auto &con_win : windows_)
        {
            con_win.second->draw();
        }
    }

    void DeviceManager::drawMenu()
    {
        std::vector<std::string> delayed_delete;
        if (ImGui::BeginMenu("Connections"))
        {
            for (auto &con_win : windows_)
            {
                ImGui::PushID(con_win.second.get());
                if (ImGui::BeginMenu(con_win.second->name().c_str()))
                {
                    ImGui::MenuItem("Show/Hide Window", "", &con_win.second->openState());
                    if (ImGui::MenuItem("Close Connection"))
                    {
                        delayed_delete.push_back(con_win.first);
                    }
                    ImGui::EndMenu();
                }
                ImGui::PopID();
            }
            ImGui::EndMenu();
        }

        for (const auto &id : delayed_delete)
        {
            windows_.erase(id);
            if (auto it = connections_.find(id); it != connections_.end())
            {
                it->second->disconnect();
            }
            connections_.erase(id);
            devices_.erase(id);
        }
    }

    void DeviceManager::drawOpenRecent()
    {
        std::shared_lock<std::shared_mutex> l(recent_cons_mtx_);
        for (const auto &con : recent_connections_)
        {
            if (ImGui::MenuItem(con.options->identifier.c_str()))
            {
                if (con.connection_type == connection::Serial::kType)
                    addSerialConnection(*std::static_pointer_cast<connection::SerialOptions>(con.options), false);
                else if (con.connection_type == connection::Tcp::kType)
                    addTcpConnection(*std::static_pointer_cast<connection::TcpOptions>(con.options), false);
                else if (con.connection_type == connection::Udp::kType)
                    addUdpConnection(*std::static_pointer_cast<connection::UdpOptions>(con.options), false);
                else
                    SPDLOG_WARN("Unknown connection type: {}", con.connection_type);
            }
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

    std::future<std::tuple<bool, std::string, std::string>> DeviceManager::testSerialConnection(const connection::SerialOptions &opts)
    {
        return std::async([=]() {
            connection_test_buffer_.clear();
            auto con = std::make_shared<connection::Serial>(*connection_test_handle_, io_context_, "connection_test_serial");

            try
            {
                spdlog::debug("test connection for serial device at {}@{}", opts.port, opts.baud_rate);
                con->setOptions(opts);
            }
            catch (const std::exception &err)
            {
                SPDLOG_ERROR("while connecting to serial device {}. with error: {}", opts.port, err.what());
                return std::make_tuple(false, std::string(err.what()), connection_test_buffer_);
            }
            con->connect();
            if (!con->isConnected())
            {
                return std::make_tuple(false, fmt::format("Could set options but couldn't connect to {}@{}", opts.port, opts.baud_rate), connection_test_buffer_);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            con->disconnect();
            return std::make_tuple(true, fmt::format("Connected to {}@{}", opts.port, opts.baud_rate), connection_test_buffer_);
        });
    }

    std::pair<bool, std::string> DeviceManager::addSerialConnection(const connection::SerialOptions &opts, const bool add_to_recents)
    {
        if (auto it = connections_.find(opts.identifier); it != connections_.end())
        {
            return std::make_pair(false, "Connection with identifier already exists");
        }
        auto &dev_instance = devices_[opts.identifier] = std::make_unique<DeviceConnection>();
        auto con = std::make_shared<connection::Serial>(*dev_instance, io_context_, opts.identifier);
        try
        {
            spdlog::debug("add connection for serial device at {}@{}", opts.port, opts.baud_rate);
            con->setOptions(opts);
        }
        catch (const std::exception &err)
        {
            SPDLOG_ERROR("while adding serial device {}. with error: {}", opts.port, err.what());
            devices_.erase(opts.identifier);
            return std::make_pair(false, fmt::format("while adding serial device {}. with error: {}", opts.port, err.what()));
        }

        windows_.emplace(opts.identifier, std::make_unique<SerialConnectionWin>(con, *dev_instance));
        if (add_to_recents)
        {
            config::addRecentConnection(kDefaultRecentConnectionsFile, con->type(), con->serialOptions());
            refreshRecents();
        }
        connections_.emplace(opts.identifier, std::move(con));
        return std::make_pair(true, "Connection added");
    }

    std::future<std::tuple<bool, std::string, std::string>> DeviceManager::testTcpConnection(const connection::TcpOptions &opts)
    {
        return std::async([=]() {
            connection_test_buffer_.clear();
            auto con = std::make_shared<connection::Tcp>(*connection_test_handle_, io_context_, "connection_test_tcp");

            try
            {
                spdlog::debug("test tcp connection {}:{}", opts.server, opts.server_port);
                con->setOption(opts.server, opts.server_port, opts.service);
                con->setOption(opts.packet_end);
                con->connect();
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
            catch (const std::exception &err)
            {
                SPDLOG_ERROR("while connecting to tcp connection {}:{}. with error: {}", opts.server, opts.server_port, err.what());
                return std::make_tuple(false, std::string(err.what()), connection_test_buffer_);
            }
            if (!con->isConnected())
            {
                return std::make_tuple(false, fmt::format("Could set options but couldn't connect to {}:{}", opts.server, opts.server_port), connection_test_buffer_);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            con->disconnect();
            return std::make_tuple(true, fmt::format("Connected to {}:{}", opts.server, opts.server_port), connection_test_buffer_);
        });
    }

    std::pair<bool, std::string> DeviceManager::addTcpConnection(const connection::TcpOptions &opts, const bool add_to_recents)
    {
        if (auto it = connections_.find(opts.identifier); it != connections_.end())
        {
            return std::make_pair(false, "tcp connection identifier already exits. please close it first");
        }
        auto &dev_instance = devices_[opts.identifier] = std::make_unique<DeviceConnection>();
        auto con = std::make_shared<connection::Tcp>(*dev_instance, io_context_, opts.identifier);
        try
        {
            con->setOption(opts.server, opts.server_port, opts.service);
            con->setOption(opts.packet_end);
            con->applyOptions();
        }
        catch (const std::exception &err)
        {
            devices_.erase(opts.identifier);
            return std::make_pair(false, fmt::format("tcp connection with error: {}", opts.identifier, err.what()));
        }

        windows_.emplace(opts.identifier, std::make_unique<TcpConnectionWin>(con, *dev_instance));
        if (add_to_recents)
        {
            config::addRecentConnection(kDefaultRecentConnectionsFile, con->type(), con->tcpOptions());
            refreshRecents();
        }
        connections_.emplace(opts.identifier, std::move(con));

        return std::make_pair(true, "Connection established");
    }

    std::pair<bool, std::string> DeviceManager::addUdpConnection(const connection::UdpOptions &opts, const bool add_to_recents)
    {
        if (auto it = connections_.find(opts.identifier); it != connections_.end())
        {
            return std::make_pair(false, "udp connection identifier already exits. please close it first");
        }
        auto &dev_instance = devices_[opts.identifier] = std::make_unique<DeviceConnection>();
        auto con = std::make_shared<connection::Udp>(*dev_instance, io_context_, opts.identifier);
        try
        {
            con->setOption(opts.listen_port, opts.listen_protocol);
            con->setOption(opts.write_address, opts.write_port);
            con->applyOptions();
        }
        catch (const std::exception &err)
        {
            devices_.erase(opts.identifier);
            return std::make_pair(false, fmt::format("udp connection with error: {}", opts.identifier, err.what()));
        }
        windows_.emplace(opts.identifier, std::make_unique<UdpConnectionWin>(con, *dev_instance));
        if (add_to_recents)
        {
            config::addRecentConnection(kDefaultRecentConnectionsFile, con->type(), con->udpOptions());
            refreshRecents();
        }
        connections_.emplace(opts.identifier, std::move(con));
        return std::make_pair(true, "Connection established");
    }

    void DeviceManager::refreshRecents()
    {
        std::unique_lock<std::shared_mutex> l(recent_cons_mtx_);
        recent_connections_ = config::recentConnections(kDefaultRecentConnectionsFile);
    }

    DeviceManager::~DeviceManager()
    {
        should_stop_ = true;
        for (auto &con : connections_)
            con.second->disconnect();

        io_context_.stop();
        if (io_thread_.joinable())
        {
            io_thread_.join();
        }
    }
} // namespace gui
