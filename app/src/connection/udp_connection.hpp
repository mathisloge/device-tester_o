#pragma once
#include <memory>
#include <array>
#include <boost/asio.hpp>
#include "connection.hpp"
class UdpConnection : public std::enable_shared_from_this<UdpConnection>, public Connection
{
    using udp = boost::asio::ip::udp;

public:
    enum class Protocol
    {
        none,
        ipv4,
        ipv6       
    };

public:
    explicit UdpConnection(ConnectionHandle &handle, const std::string &identifier, boost::asio::io_context &io_context);
    bool isConnected() const override;
    void connect() override;
    void disconnect() override;

    void setOption(const std::string &write_address, const unsigned short send_port);
    void setOption(const unsigned short listen_port, const Protocol protocol = Protocol::ipv4);

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
     udp::endpoint  remote_endpoint_;
    bool should_receive_;

    unsigned short listen_port_;
    Protocol listen_protocol_;
};
