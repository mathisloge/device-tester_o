#include "tcp_connection.hpp"
#include <fmt/format.h>
#include <spdlog/spdlog.h>
TcpConnection::TcpConnection(ConnectionHandle &handle, const std::string &identifier, boost::asio::io_context &io_context)
    : Connection(handle, identifier),
      strand_{asio::make_strand(io_context)},
      packet_end_{'\n'},
      should_run_{true}
{
}
bool TcpConnection::isConnected() const
{
}
void TcpConnection::connect()
{
    tcp::resolver resolver(strand_);
    const auto network_target = fmt::format("{}:{}", server_, server_port_);

    // Start the connect actor.
    endpoints_ = resolver.resolve(network_target, service_);
    beginConnect(endpoints_.begin());
}
void TcpConnection::disconnect()
{
    socket_.close();
}
void TcpConnection::setOption(const std::string &server, const unsigned short server_port, const std::string &service = "")
{
    server_ = server;
    server_port_ = server_port;
    service_ = service;
}

void TcpConnection::setOption(char packet_end)
{
    packet_end_ = packet_end;
}

void TcpConnection::beginConnect(tcp::resolver::results_type::iterator endpoint_iter)
{
    if (endpoint_iter != endpoints_.end())
    {
        SPDLOG_INFO("Connecting to {}...", endpoint_iter->endpoint());
        socket_.async_connect(endpoint_iter->endpoint(), std::bind(&TcpConnection::handleConnect, this, std::placeholder::_1, endpoint_iter));
    }
    else
        disconnect();
}

void TcpConnection::handleConnect(const boost::system::error_code &error, tcp::resolver::results_type::iterator endpoint_iter)
{
    if (!should_run_)
        return;

    if (!socket_.is_open())
    {
        SPDLOG_INFO("connect timed out");
        beginConnect(++endpoint_iter);
    }
    else if (error)
    {
        SPDLOG_INFO("Connect error: {}", error.message());
        disconnect();
        start_connect(++endpoint_iter);
    }
    else
    {
        SPDLOG_INFO("Connected to: {}", endpoint_iter->endpoint());
        startRead();
        startWrite();
    }
}

void TcpConnection::startRead()
{
    boost::asio::async_read_until(socket_,
                                  boost::asio::dynamic_buffer(buffer_rx_), packet_end_,
                                  std::bind(&TcpConnection::handleRead, this, std::placeholders::_1, std::placeholders::_2));
}
void TcpConnection::startWrite()
{
    // todo add write buffer
#if 0
    if (should_run_)
        return;
    boost::asio::async_write(socket_, boost::asio::buffer(buffer_tx_),
                             std::bind(&TcpConnection::handleWrite, this, std::placeholders::_1));
#endif
}

void TcpConnection::handleRead(const boost::system::error_code &error, std::size_t n)
{
    if (should_run_)
        return;

    if (!error)
    {
        handle_.processData(std::span<uint8_t>(input_buffer_.begin(), input_buffer_.end());
        input_buffer_.clear();
        startRead();
    }
    else
    {
        SPDLOG_ERROR("Error while receiving {}", error.message());
        disconnect();
    }
}

void TcpConnection::handleWrite(const boost::system::error_code &error)
{
    if (should_run_)
        return;

    if (!error)
    {
        startWrite();
    }
    else
    {
        SPDLOG_ERROR("Error while writing {}", error.message());
        disconnect();
    }
}
