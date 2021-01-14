#include "tcp_connection.hpp"
#include <fmt/format.h>
#include <spdlog/spdlog.h>
TcpConnection::TcpConnection(ConnectionHandle &handle, const std::string &identifier, boost::asio::io_context &io_context)
    : Connection(handle, identifier),
      strand_{boost::asio::make_strand(io_context)},
      packet_end_{'\n'},
      should_run_{true},
      socket_{strand_},
      is_connected_{false}
{
}
bool TcpConnection::isConnected() const
{
    return is_connected_ && socket_.is_open();
}
void TcpConnection::connect()
{
    disconnect();
    boost::asio::io_context resolver_ctx;
    tcp::resolver resolver(resolver_ctx);
    // Start the connect actor.
    if (service_.empty())
        endpoints_ = resolver.resolve(server_, fmt::format("{}", server_port_));
    else
    {
        const auto network_target = (server_port_ == 0) ? server_ : fmt::format("{}:{}", server_, server_port_);
        endpoints_ = resolver.resolve(network_target, service_);
    }
    should_run_ = true;
    beginConnect(endpoints_.begin());
}
void TcpConnection::disconnect()
{
    if (!isConnected())
    {
        should_run_ = false;
        return;
    }
    SPDLOG_INFO("Disconnecting from {}:{}", server_, server_port_);
    is_connected_ = false;
    should_run_ = false;
    socket_.close();
}
void TcpConnection::setOption(const std::string &server, const unsigned short server_port, const std::string &service)
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
        SPDLOG_INFO("Connecting to {}...", endpoint_iter->endpoint().address().to_string());
        socket_.async_connect(endpoint_iter->endpoint(), std::bind(&TcpConnection::handleConnect, shared_from_this(), std::placeholders::_1, endpoint_iter));
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
        SPDLOG_ERROR("Connect error: {}", error.message());
        disconnect();
        beginConnect(++endpoint_iter);
    }
    else
    {
        SPDLOG_INFO("Connected to: {}", endpoint_iter->endpoint().address().to_string());
        is_connected_ = true;
        startRead();
        startWrite();
    }
}

void TcpConnection::startRead()
{
    boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(buffer_rx_), packet_end_,
                                  std::bind(&TcpConnection::handleRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}
void TcpConnection::startWrite()
{
    // todo add write buffer
#if 0
    if (!should_run_)
        return;
    boost::asio::async_write(socket_, boost::asio::buffer(buffer_tx_),
                             std::bind(&TcpConnection::handleWrite, shared_from_this(), std::placeholders::_1));
#endif
}

void TcpConnection::handleRead(const boost::system::error_code &error, std::size_t length /*length*/)
{
    if (!error)
    {
        handle_.processData(std::span<uint8_t>(buffer_rx_.begin(), buffer_rx_.end()));
        buffer_rx_.clear();
        if (should_run_)
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
    if (!should_run_)
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
