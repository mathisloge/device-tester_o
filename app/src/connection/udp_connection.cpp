#include "udp_connection.hpp"
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
  should_receive_ = true;
  startRead();
}
void UdpConnection::disconnect()
{
  should_receive_ = false;
}

void UdpConnection::setOption(const std::string &write_address, const unsigned short send_port)
{
}
void UdpConnection::setOption(const unsigned short resv_port)
{
}

void UdpConnection::startRead()
{
  socket_.async_receive_from(
      boost::asio::buffer(buffer_rx_),
      rx_endpoint_,
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
void UdpConnection::handleWrite(const boost::system::error_code &error)
{
}
