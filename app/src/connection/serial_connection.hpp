#pragma once
#include <memory>
#include <array>
#include <boost/asio.hpp>
#include "connection.hpp"

class SerialConnection : public std::enable_shared_from_this<SerialConnection>, public Connection
{
    static constexpr auto kDefaultPar = boost::asio::serial_port_base::parity::none;
    static constexpr auto kDefaultCharSize = 8;
    static constexpr auto kDefaultFlow = boost::asio::serial_port_base::flow_control::none;
    static constexpr auto kDefaultStopBits = boost::asio::serial_port_base::stop_bits::one;

public:
    explicit SerialConnection(ConnectionHandle &handle, boost::asio::io_context& io_context);

    void open(const std::string &devname,
              unsigned int baud_rate,
              boost::asio::serial_port_base::parity parity = boost::asio::serial_port_base::parity(kDefaultPar),
              boost::asio::serial_port_base::character_size char_size = boost::asio::serial_port_base::character_size(kDefaultCharSize),
              boost::asio::serial_port_base::flow_control flow_ctrl = boost::asio::serial_port_base::flow_control(kDefaultFlow),
              boost::asio::serial_port_base::stop_bits stop_bits = boost::asio::serial_port_base::stop_bits(kDefaultStopBits)) noexcept;
    ~SerialConnection();

private:
    void handleRead(const boost::system::error_code &e, std::size_t bytes_transferred);
    void processData();

private:
    /// Strand to ensure the connection's handlers are not called concurrently.
    boost::asio::strand<boost::asio::executor> strand_;
    boost::asio::serial_port serial_;

    std::array<uint8_t, 64> rx_buffer_;
};