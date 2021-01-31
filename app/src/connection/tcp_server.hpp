#pragma once
#include "connection.hpp"
#include <boost/asio.hpp>
#include "tcp_server_manager.hpp"
#include "tcp_server_connection_handle.hpp"
namespace connection
{
    struct TcpServerOptions : Options
    {
        enum class Protocol
        {
            ipv4,
            ipv6
        };
        Protocol protocol;
        unsigned short port;
    };

    class TcpServer final : public std::enable_shared_from_this<TcpServer>, public Connection
    {
        using tcp = boost::asio::ip::tcp;

    public:
        static constexpr std::string_view kType = "tcp-server";

    public:
        explicit TcpServer(TcpServerConnectionHandle &handle, boost::asio::io_context &io_context, const std::string &identifier);
        bool isConnected() const override;
        void connect() override;
        void disconnect() override;
        void write(std::span<uint8_t> data) override;
        void applyOptions() override;
        void setOptions(const TcpServerOptions& options);
        const Options &options() const override;
        std::string_view type() const override;
        ~TcpServer();

    private:
        void startAccept();

    private:
        /// Strand to ensure the connection handlers are not called concurrently.
        boost::asio::strand<boost::asio::executor> strand_;
        tcp::acceptor acceptor_;
        TcpServerConnectionHandle& conn_handle_;
        TcpServerOptions options_;
        TcpServerManager manager_;
    };
} // namespace connection
