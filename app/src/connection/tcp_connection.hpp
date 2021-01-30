#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "connection.hpp"

namespace connection
{
    struct TcpOptions : Options
    {
        std::string server;
        unsigned short server_port;
        std::string service;
        char packet_end;
    };

    class Tcp : public std::enable_shared_from_this<Tcp>, public Connection
    {
        using tcp = boost::asio::ip::tcp;

    public:
        static constexpr std::string_view kType = "tcp";

    public:
        explicit Tcp(ConnectionHandle &handle, boost::asio::io_context &io_context, const std::string &identifier);
        bool isConnected() const override;
        void connect() override;
        void disconnect() override;
        void write(std::span<uint8_t> data) override;
        void setOption(const std::string &server, const unsigned short server_port, const std::string &service = "");
        void setOption(char packet_end);
        const Options &options() const override;
        const TcpOptions &tcpOptions() const;
        std::string_view type() const override;

    private:
        void beginConnect(tcp::resolver::results_type::iterator endpoint_iter);
        void handleConnect(const boost::system::error_code &error, tcp::resolver::results_type::iterator endpoint_iter);
        void startRead();
        void startWrite();
        void handleRead(const boost::system::error_code &error, std::size_t n);
        void handleWrite(const boost::system::error_code &error);

    private:
        /// Strand to ensure the connection's handlers are not called concurrently.
        boost::asio::strand<boost::asio::executor> strand_;
        std::vector<uint8_t> buffer_rx_;
        TcpOptions options_;

        tcp::socket socket_;
        tcp::resolver::results_type endpoints_;
        bool should_run_;

        bool is_connected_;
    };
} // namespace connection
