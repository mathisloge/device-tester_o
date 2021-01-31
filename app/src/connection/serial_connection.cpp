#include "serial_connection.hpp"
#include <spdlog/spdlog.h>

namespace asio = boost::asio;

namespace connection
{

    SerialOptions::SerialOptions()
        : baud_rate(19200),
          parity(kDefaultPar),
          char_size(kDefaultCharSize),
          flow_control(kDefaultFlow),
          stop_bits(kDefaultStopBits)
    {
    }
    SerialOptions::SerialOptions(const std::string &identifier)
        : Options{identifier},
          baud_rate(19200),
          parity(kDefaultPar),
          char_size(kDefaultCharSize),
          flow_control(kDefaultFlow),
          stop_bits(kDefaultStopBits)
    {
    }

    Serial::Serial(ConnectionHandle &handle, boost::asio::io_context &io_context, const std::string &identifier)
        : Connection(handle),
          strand_{asio::make_strand(io_context)},
          serial_{strand_},
          options_{identifier}
    {
    }

    void Serial::handleRead(const boost::system::error_code &e, std::size_t bytes_transferred)
    {
        if (!e)
        {
            handle_.processData(std::span<uint8_t>{rx_buffer_.begin(), bytes_transferred});
            serial_.async_read_some(boost::asio::buffer(rx_buffer_),
                                    std::bind(&Serial::handleRead, shared_from_this(),
                                              std::placeholders::_1,
                                              std::placeholders::_2));
        }
        else
        {
            //! \todo log error in own error
            spdlog::error("error while reading from serial device: {}", e.message());
        }
    }

    void Serial::handleWrite(std::shared_ptr<std::vector<uint8_t>> buffer_tx,
                             const boost::system::error_code &error,
                             std::size_t bytes_transferred)
    {
        if (!error)
        {
            if (buffer_tx->size() != bytes_transferred)
            {
                SPDLOG_DEBUG("couldn't send all bytes");
                buffer_tx->erase(buffer_tx->begin(), buffer_tx->begin() + bytes_transferred);
                serial_.async_write_some(boost::asio::buffer(*buffer_tx), std::bind(&Serial::handleWrite, shared_from_this(), buffer_tx, std::placeholders::_1, std::placeholders::_2));
            }
        }
        else
        {
            SPDLOG_ERROR("Couldn't write {} bytes", buffer_tx->size());
        }
    }

    bool Serial::isConnected() const
    {
        return serial_.is_open();
    }
    void Serial::connect()
    {
        open();
        serial_.async_read_some(boost::asio::buffer(rx_buffer_),
                                std::bind(&Serial::handleRead, shared_from_this(),
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    }
    void Serial::open()
    {
        if (!serial_.is_open())
            serial_.open(options_.port);
        applyOptions();
    }
    void Serial::disconnect()
    {
        if (serial_.is_open())
        {
            spdlog::debug("closing serial connection {}", options_.port);
            serial_.cancel();
            serial_.close();
        }
    }

    void Serial::write(std::span<uint8_t> data)
    {
        if (!serial_.is_open())
        {
            SPDLOG_ERROR("port not opended");
            return;
        }
        std::shared_ptr<std::vector<uint8_t>> buffer_tx = std::make_shared<std::vector<uint8_t>>(data.begin(), data.end());
        serial_.async_write_some(boost::asio::buffer(*buffer_tx), std::bind(&Serial::handleWrite, shared_from_this(), buffer_tx, std::placeholders::_1, std::placeholders::_2));
    }

    void Serial::applyOptions()
    {
        // apply port only if we were connected before. otherwise the user is expected to connect.
        if (isConnected())
        {
            disconnect();
            open();
        }
        serial_.set_option(boost::asio::serial_port_base::baud_rate(options_.baud_rate));
        serial_.set_option(options_.parity);
        serial_.set_option(options_.char_size);
        serial_.set_option(options_.flow_control);
        serial_.set_option(options_.stop_bits);
    }

    void Serial::setOptions(const SerialOptions &options)
    {
        setOption(options.port);
        setOption(options.baud_rate);
        setOption(options.parity);
        setOption(options.char_size);
        setOption(options.flow_control);
        setOption(options.stop_bits);
    }
    void Serial::setOption(const std::string &devname)
    {
        options_.port = devname;
    }
    void Serial::setOption(unsigned int baud_rate)
    {
        options_.baud_rate = baud_rate;
    }
    void Serial::setOption(const boost::asio::serial_port_base::parity &parity)
    {
        options_.parity = parity;
    }
    void Serial::setOption(const boost::asio::serial_port_base::character_size &char_size)
    {
        options_.char_size = char_size;
    }
    void Serial::setOption(const boost::asio::serial_port_base::flow_control &flow_control)
    {
        options_.flow_control = flow_control;
    }
    void Serial::setOption(const boost::asio::serial_port_base::stop_bits &stop_bits)
    {
        options_.stop_bits = stop_bits;
    }

    const SerialOptions &Serial::serialOptions() const
    {
        return options_;
    }

    const connection::Options &Serial::options() const
    {
        return options_;
    }

    std::string_view Serial::type() const
    {
        return kType;
    }

    Serial::~Serial()
    {
        disconnect();
    }

} // namespace connection
