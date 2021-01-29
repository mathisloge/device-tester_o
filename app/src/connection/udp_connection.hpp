#pragma once
#include <memory>
#include <array>
#include <boost/asio.hpp>
#include "connection.hpp"

namespace connection
{

    struct UdpOptions : public Options
    {
        enum class Protocol
        {
            none,
            ipv4,
            ipv6
        };
        std::string write_address;
        unsigned short write_port;
        Protocol listen_protocol;
        unsigned short listen_port;
    };
    class Udp : public std::enable_shared_from_this<Udp>, public Connection
    {
        using udp = boost::asio::ip::udp;

    public:
        static constexpr std::string_view kType = "udp";

    public:
        explicit Udp(ConnectionHandle &handle, boost::asio::io_context &io_context, const std::string &identifier);
        bool isConnected() const override;
        void connect() override;
        void disconnect() override;

        void setOption(const std::string &write_address, const unsigned short write_port);
        void setOption(const unsigned short listen_port, const UdpOptions::Protocol protocol = UdpOptions::Protocol::ipv4);
        const connection::Options &options() const override;
        const UdpOptions &udpOptions() const;
        std::string_view type() const override;

    private:
        void startRead();
        void startWrite();
        void handleRead(const boost::system::error_code &error, std::size_t n);
        void handleWrite(const boost::system::error_code &error);

    private:
        /// Strand to ensure the connection's handlers are not called concurrently.
        boost::asio::strand<boost::asio::executor> strand_;
        udp::socket socket_;
        std::array<uint8_t, 1024> buffer_rx_;
        udp::endpoint listen_endpoint_;
        udp::endpoint remote_endpoint_;
        bool should_receive_;

        UdpOptions options_;
    };
} // namespace connection
