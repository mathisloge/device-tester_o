#include "serial_connection.hpp"
namespace asio = boost::asio;

SerialConnection::SerialConnection(asio::executor &io_executor)
    : strand_{asio::make_strand(io_executor)}, serial_{strand_}
{
}

void SerialConnection::open()
{
}
void SerialConnection::close()
{
}

void SerialConnection::receiveAndDispatch()
{
    //serial_.async_read_some()
}

void SerialConnection::handleRead(const boost::system::error_code &e, std::size_t bytes_transferred)
{
    if (!e)
    {
        serial_.async_read_some(boost::asio::buffer(rx_buffer_),
                                std::bind(&SerialConnection::handleRead, shared_from_this(),
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    }
}