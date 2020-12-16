#include "serial_connection.hpp"
#include <spdlog/spdlog.h>

namespace asio = boost::asio;

SerialConnection::SerialConnection(ConnectionHandle &handle,
                                   boost::asio::executor &io_executor,
                                   const std::string &devname,
                                   unsigned int baud_rate,
                                   boost::asio::serial_port_base::parity parity,
                                   boost::asio::serial_port_base::character_size char_size,
                                   boost::asio::serial_port_base::flow_control flow_ctrl,
                                   boost::asio::serial_port_base::stop_bits stop_bits)
    : Connection(handle),
      strand_{asio::make_strand(io_executor)},
      serial_{strand_}
{
    open(devname, baud_rate, parity, char_size, flow_ctrl, stop_bits);
}

void SerialConnection::handleRead(const boost::system::error_code &e, std::size_t bytes_transferred)
{
    if (!e)
    {
        handle_.processData(&rx_buffer_[0], bytes_transferred);
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
//! \todo move the try and errors in one single function...
void SerialConnection::open(const std::string &devname,
                            unsigned int baud_rate,
                            boost::asio::serial_port_base::parity parity,
                            boost::asio::serial_port_base::character_size char_size,
                            boost::asio::serial_port_base::flow_control flow_ctrl,
                            boost::asio::serial_port_base::stop_bits stop_bits) noexcept
{
    try
    {
        serial_.set_option(parity);
    }
    catch (const boost::system::system_error &e)
    {
        spdlog::error("Device: {}, error setting parity: {}", devname, e.what());
    }
    try
    {
        serial_.set_option(char_size);
    }
    catch (const boost::system::system_error &e)
    {
        spdlog::error("Device: {}, error setting char size: {}", devname, e.what());
    }
    try
    {
        serial_.set_option(flow_ctrl);
    }
    catch (const boost::system::system_error &e)
    {
        spdlog::error("Device: {}, error setting flow control: {}", devname, e.what());
    }
    try
    {
        serial_.set_option(stop_bits);
    }
    catch (const boost::system::system_error &e)
    {
        spdlog::error("Device: {}, error setting stop bits: {}", devname, e.what());
    }
    try
    {
        serial_.open(devname);
    }
    catch (const boost::system::system_error &e)
    {
        spdlog::error("error can't open serial connection to device [{}] with error: {}", devname, e.what());
    }
    try
    {
        serial_.async_read_some(boost::asio::buffer(rx_buffer_),
                                std::bind(&SerialConnection::handleRead, shared_from_this(),
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    }
    catch (std::exception e)
    {
        spdlog::error("unknown error while starting async read. {}", e.what());
    }
}

SerialConnection::~SerialConnection()
{
    if (serial_.is_open())
    {
        spdlog::debug("closing serial connection on destruct");
        serial_.close();
    }
}