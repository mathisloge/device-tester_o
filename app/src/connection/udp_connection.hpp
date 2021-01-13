#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "connection.hpp"
class UdpConnection : public std::enable_shared_from_this<UdpConnection>, public Connection
{
    using udp = boost::asio::ip::udp;

public:
    explicit UdpConnection(ConnectionHandle &handle, const std::string &identifier, boost::asio::io_context &io_context);
    bool isConnected() const override;
    void connect() override;
    void disconnect() override;

    void setOption(const std::string &target, const unsigned short send_port);
    void setOption(const unsigned short resv_port);

private:
};
