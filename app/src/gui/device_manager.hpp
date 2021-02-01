#pragma once
#include <future>
#include <tuple>
#include <thread>
#include <vector>
#include <shared_mutex>
#include <boost/asio.hpp>
#include "../config/recent.hpp"
#include "../connection/connection.hpp"
#include "../connection/serial_connection.hpp"
#include "../connection/tcp_connection.hpp"
#include "../connection/udp_connection.hpp"
#include "../devices/device_connection.hpp"
#include "windows/connection_win.hpp"
namespace gui
{
    class DumbConnectionHandle;
    class DeviceManager final
    {
    public:
        using DeviceMap = std::map<std::string, std::unique_ptr<DeviceConnection>>;
        using ConnectionMap = std::map<std::string, std::shared_ptr<connection::Connection>>;
        using WindowMap = std::map<std::string, std::unique_ptr<ConnectionWin>>;

    public:
        DeviceManager();
        void draw();
        void drawMenu();
        void drawOpenRecent();
        std::future<std::tuple<bool, std::string, std::string>> testSerialConnection(const connection::SerialOptions &opts);
        std::pair<bool, std::string> addSerialConnection(const connection::SerialOptions &opts, const bool add_to_recents = true);

        std::future<std::tuple<bool, std::string, std::string>> testTcpConnection(const connection::TcpOptions &opts);
        std::pair<bool, std::string> addTcpConnection(const connection::TcpOptions &opts, const bool add_to_recents = true);

        std::pair<bool, std::string> addUdpConnection(const connection::UdpOptions &opts, const bool add_to_recents = true);

        void closeConnection();
        ~DeviceManager();

    private:
        void refreshRecents();
        void ioThread();
        void startIdleTimer();

    private:
        bool should_stop_;
        boost::asio::io_context io_context_;
        boost::asio::steady_timer io_idle_timer_;
        std::thread io_thread_;

        mutable std::shared_mutex recent_cons_mtx_;
        std::vector<config::RecentConnection> recent_connections_;

        DeviceMap devices_;
        ConnectionMap connections_;
        WindowMap windows_;

        std::unique_ptr<DumbConnectionHandle> connection_test_handle_;
        std::string connection_test_buffer_;
    };
} // namespace gui
