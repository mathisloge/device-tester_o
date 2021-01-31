#pragma once
#include <memory>
#include <array>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include "tcp_server_connection_handle.hpp"
namespace connection
{
    class TcpServerManager;
    class TcpServerClient final : public boost::noncopyable, public std::enable_shared_from_this<TcpServerClient>
    {
        using tcp = boost::asio::ip::tcp;

    public:
        explicit TcpServerClient(tcp::socket socket, TcpServerManager &manager, TcpServerConnectionHandle &handler);
        void start();
        void stop();
        void shutdown();
        void write(std::span<uint8_t> data);

        ~TcpServerClient();
    private:
        void startRead();

    private:
        tcp::socket socket_;
        TcpServerManager &manager_;
        TcpServerConnectionHandle &handler_;
        std::array<uint8_t, 1024> rx_buffer_;
    };

    using TcpServerClientPtr = std::shared_ptr<TcpServerClient>;
} // namespace connection
