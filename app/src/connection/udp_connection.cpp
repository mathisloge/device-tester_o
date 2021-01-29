#include "udp_connection.hpp"
#include <span>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

namespace connection
{
  Udp::Udp(ConnectionHandle &handle, boost::asio::io_context &io_context, const std::string &identifier)
      : Connection(handle),
        strand_{boost::asio::make_strand(io_context)},
        socket_{strand_},
        options_{identifier}
  {
  }

  bool Udp::isConnected() const
  {
    return should_receive_;
  }

  void Udp::connect()
  {
    if (options_.listen_protocol != UdpOptions::Protocol::none)
    {
      should_receive_ = true;
      listen_endpoint_ = udp::endpoint(options_.listen_protocol == UdpOptions::Protocol::ipv4 ? udp::v4() : udp::v6(), options_.listen_port);
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
  void Udp::disconnect()
  {
    should_receive_ = false;
    socket_.close();
  }

  void Udp::setOption(const std::string & /*write_address*/, const unsigned short /*send_port*/)
  {
  }
  void Udp::setOption(const unsigned short listen_port, const UdpOptions::Protocol protocol)
  {
    options_.listen_port = listen_port;
    options_.listen_protocol = protocol;
  }

  void Udp::startRead()
  {
    socket_.async_receive_from(
        boost::asio::buffer(buffer_rx_),
        remote_endpoint_,
        std::bind(&Udp::handleRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
  }
  void Udp::startWrite()
  {
  }

  void Udp::handleRead(const boost::system::error_code &error, std::size_t n)
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
  void Udp::handleWrite(const boost::system::error_code &)
  {
  }

  const Options &Udp::options() const
  {
    return options_;
  }

  const UdpOptions &Udp::udpOptions() const
  {
    return options_;
  }

  std::string_view Udp::type() const
  {
    return kType;
  }
} // namespace connection
