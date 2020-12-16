#pragma once
#include <memory>
#include <array>
#include <boost/asio.hpp>
#include "connection.hpp"

class SerialConnection : public std::enable_shared_from_this<SerialConnection>, public Connection
{
public:
    explicit SerialConnection(boost::asio::executor &io_executor);
    void open() override;
    void close() override;

private:
    void receiveAndDispatch();
    void handleRead(const boost::system::error_code &e, std::size_t bytes_transferred);

private:
    /// Strand to ensure the connection's handlers are not called concurrently.
    boost::asio::strand<boost::asio::executor> strand_;
    boost::asio::serial_port serial_;

    std::array<uint8_t, 64> rx_buffer_;
};