#include "serial_connection.hpp"
#include <spdlog/spdlog.h>

namespace asio = boost::asio;

SerialConnection::SerialConnection(ConnectionHandle &handle, const std::string &identifier, boost::asio::io_context &io_context)
    : Connection(handle, identifier),
      strand_{asio::make_strand(io_context)},
      serial_{strand_}
{
}

void SerialConnection::handleRead(const boost::system::error_code &e, std::size_t bytes_transferred)
{
    if (!e)
    {
        handle_.processData(std::span<uint8_t>{rx_buffer_.begin(), bytes_transferred});
        serial_.async_read_some(boost::asio::buffer(rx_buffer_),
                                std::bind(&SerialConnection::handleRead, shared_from_this(),
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    }
    else
    {
        //! \todo log error in own error
        spdlog::error("error while reading from serial device: {}", e.message());
    }
}

bool SerialConnection::isConnected() const
{
    return serial_.is_open();
}
void SerialConnection::connect()
{
    open();
    serial_.async_read_some(boost::asio::buffer(rx_buffer_),
                            std::bind(&SerialConnection::handleRead, shared_from_this(),
                                      std::placeholders::_1,
                                      std::placeholders::_2));
}
void SerialConnection::open()
{
    if (!serial_.is_open())
        serial_.open(dev_name_);
}
void SerialConnection::disconnect()
{
    if (serial_.is_open())
    {
        spdlog::debug("closing serial connection {}", dev_name_);
        serial_.cancel();
        serial_.close();
    }
}

//! \todo move the try and errors in one single function...
void SerialConnection::setOptions(const std::string &devname,
                                  unsigned int baud_rate,
                                  boost::asio::serial_port_base::parity parity,
                                  boost::asio::serial_port_base::character_size char_size,
                                  boost::asio::serial_port_base::flow_control flow_ctrl,
                                  boost::asio::serial_port_base::stop_bits stop_bits)
{
    setOption(devname);
    setOption(baud_rate);
    setOption(parity);
    setOption(char_size);
    setOption(flow_ctrl);
    setOption(stop_bits);
    serial_.async_read_some(boost::asio::buffer(rx_buffer_),
                            std::bind(&SerialConnection::handleRead, shared_from_this(),
                                      std::placeholders::_1,
                                      std::placeholders::_2));
}
void SerialConnection::setOption(const std::string &devname)
{
    dev_name_ = devname;
    disconnect();
    open();
}
void SerialConnection::setOption(unsigned int baud_rate)
{
    serial_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
}
void SerialConnection::setOption(const boost::asio::serial_port_base::parity &parity)
{
    serial_.set_option(parity);
}
void SerialConnection::setOption(const boost::asio::serial_port_base::character_size &char_size)
{
    serial_.set_option(char_size);
}
void SerialConnection::setOption(const boost::asio::serial_port_base::flow_control &flow_control)
{
    serial_.set_option(flow_control);
}
void SerialConnection::setOption(const boost::asio::serial_port_base::stop_bits &stop_bits)
{
    serial_.set_option(stop_bits);
}

const std::string &SerialConnection::port()
{
    return dev_name_;
}

SerialConnection::~SerialConnection()
{
    disconnect();
}
