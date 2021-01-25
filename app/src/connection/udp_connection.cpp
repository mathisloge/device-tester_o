#include "udp_connection.hpp"
#include <span>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

UdpConnection::UdpConnection(ConnectionHandle &handle, const std::string &identifier, boost::asio::io_context &io_context)
    : Connection(handle, identifier),
      strand_{boost::asio::make_strand(io_context)},
      socket_{strand_}
{
}
bool UdpConnection::isConnected() const
{
  return should_receive_;
}
void UdpConnection::connect()
{
  if (listen_protocol_ != Protocol::none)
  {
    should_receive_ = true;
    listen_endpoint_ = udp::endpoint(listen_protocol_ == Protocol::ipv4 ? udp::v4() : udp::v6(), listen_port_);
    try
    {
      socket_.open(listen_endpoint_.protocol());
      socket_.bind(listen_endpoint_);
      startRead();
    }
    catch (const std::runtime_error &err)
    {
      should_receive_ = false;
      SPDLOG_ERROR("Error while binding listen port {}", err.what());
    }
  }
}
void UdpConnection::disconnect()
{
  should_receive_ = false;
  socket_.close();
}

void UdpConnection::setOption(const std::string &/*write_address*/, const unsigned short /*send_port*/)
{
}
void UdpConnection::setOption(const unsigned short listen_port, const Protocol protocol)
{
  listen_port_ = listen_port;
  listen_protocol_ = protocol;
}

void UdpConnection::startRead()
{
  socket_.async_receive_from(
      boost::asio::buffer(buffer_rx_),
      remote_endpoint_,
      std::bind(&UdpConnection::handleRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}
void UdpConnection::startWrite()
{
}

void UdpConnection::handleRead(const boost::system::error_code &error, std::size_t n)
{
  if (!error)
  {
    handle_.processData(std::span<uint8_t>(buffer_rx_.begin(), buffer_rx_.begin() + n));
    buffer_rx_.fill(0);
    if (should_receive_)
      startRead();
  }
  else
  {
    SPDLOG_ERROR("Error while receiving {}", error.message());
    disconnect();
  }
}
void UdpConnection::handleWrite(const boost::system::error_code &)
{
}
