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
        return socket_.is_open();
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

    void Udp::write(std::span<uint8_t> data)
    {
        std::shared_ptr<std::vector<uint8_t>> buffer_tx = std::make_shared<std::vector<uint8_t>>(data.begin(), data.end());
        if (!socket_.is_open())
        {
            socket_.open(udp::v4());
        }
        socket_.async_send_to(
            boost::asio::buffer(*buffer_tx),
            write_endpoint_,
            std::bind(&Udp::handleWrite, shared_from_this(), buffer_tx, std::placeholders::_1, std::placeholders::_2));
    }

    void Udp::applyOptions()
    {
        if(isConnected()) {
            disconnect();
            connect();
        }
        if (!options_.write_address.empty() && options_.write_port > 0)
            write_endpoint_ = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(options_.write_address), options_.write_port);
    }

    void Udp::setOptions(const UdpOptions &options)
    {
        setOption(options.listen_port, options.listen_protocol);
        setOption(options.write_address, options.write_port);
    }

    void Udp::setOption(const std::string &write_address, const unsigned short write_port)
    {
        options_.write_address = write_address;
        options_.write_port = write_port;
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
    void Udp::handleWrite(std::shared_ptr<std::vector<uint8_t>> buffer_tx,
                          const boost::system::error_code &error,
                          std::size_t /*bytes_transferred*/)
    {
        if (error)
        {
            SPDLOG_ERROR("Error while writing to target: {} err: {}, bytes should have been written: {}", write_endpoint_.address().to_string(), error.message(), buffer_tx->size());
        }
    }
    const Options &Udp::options() const
    {
        return options_;
    }

    const UdpOptions &Udp::udpOptions() const
    {
        return options_;
    }
    UdpOptions &Udp::udpOptions()
    {
        return options_;
    }

    std::string_view Udp::type() const
    {
        return kType;
    }
} // namespace connection
