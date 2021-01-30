#include "tcp_connection.hpp"
#include <fmt/format.h>
#include <spdlog/spdlog.h>

namespace connection
{
    Tcp::Tcp(ConnectionHandle &handle, boost::asio::io_context &io_context, const std::string &identifier)
        : Connection(handle),
          strand_{boost::asio::make_strand(io_context)},
          should_run_{true},
          socket_{strand_},
          is_connected_{false},
          options_{identifier}
    {
    }
    bool Tcp::isConnected() const
    {
        return is_connected_ && socket_.is_open();
    }
    void Tcp::connect()
    {
        disconnect();
        boost::asio::io_context resolver_ctx;
        tcp::resolver resolver(resolver_ctx);
        // Start the connect actor.
        if (options_.service.empty())
            endpoints_ = resolver.resolve(options_.server, fmt::format("{}", options_.server_port));
        else
        {
            const auto network_target = (options_.server_port == 0) ? options_.server : fmt::format("{}:{}", options_.server, options_.server_port);
            endpoints_ = resolver.resolve(network_target, options_.service);
        }
        should_run_ = true;
        beginConnect(endpoints_.begin());
    }
    void Tcp::disconnect()
    {
        if (!isConnected())
        {
            should_run_ = false;
            return;
        }
        SPDLOG_INFO("Disconnecting from {}:{}", options_.server, options_.server_port);
        is_connected_ = false;
        should_run_ = false;
        socket_.close();
    }

    void Tcp::write(std::span<uint8_t> data)
    {
    }

    void Tcp::setOption(const std::string &server, const unsigned short server_port, const std::string &service)
    {
        options_.server = server;
        options_.server_port = server_port;
        options_.service = service;
    }

    void Tcp::setOption(char packet_end)
    {
        options_.packet_end = packet_end;
    }

    void Tcp::beginConnect(tcp::resolver::results_type::iterator endpoint_iter)
    {
        if (endpoint_iter != endpoints_.end())
        {
            SPDLOG_INFO("Connecting to {}...", endpoint_iter->endpoint().address().to_string());
            socket_.async_connect(endpoint_iter->endpoint(), std::bind(&Tcp::handleConnect, shared_from_this(), std::placeholders::_1, endpoint_iter));
        }
        else
            disconnect();
    }

    void Tcp::handleConnect(const boost::system::error_code &error, tcp::resolver::results_type::iterator endpoint_iter)
    {
        if (!should_run_)
            return;

        if (!socket_.is_open())
        {
            SPDLOG_INFO("connect timed out");
            beginConnect(++endpoint_iter);
        }
        else if (error)
        {
            SPDLOG_ERROR("Connect error: {}", error.message());
            disconnect();
            beginConnect(++endpoint_iter);
        }
        else
        {
            SPDLOG_INFO("Connected to: {}", endpoint_iter->endpoint().address().to_string());
            is_connected_ = true;
            startRead();
            startWrite();
        }
    }

    void Tcp::startRead()
    {
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(buffer_rx_), options_.packet_end,
                                      std::bind(&Tcp::handleRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }
    void Tcp::startWrite()
    {
        // todo add write buffer
#if 0
    if (!should_run_)
        return;
    boost::asio::async_write(socket_, boost::asio::buffer(buffer_tx_),
                             std::bind(&Tcp::handleWrite, shared_from_this(), std::placeholders::_1));
#endif
    }

    void Tcp::handleRead(const boost::system::error_code &error, std::size_t /*length*/)
    {
        if (!error)
        {
            handle_.processData(std::span<uint8_t>(buffer_rx_.begin(), buffer_rx_.end()));
            buffer_rx_.clear();
            if (should_run_)
                startRead();
        }
        else
        {
            SPDLOG_ERROR("Error while receiving {}", error.message());
            disconnect();
        }
    }

    void Tcp::handleWrite(const boost::system::error_code &error)
    {
        if (!should_run_)
            return;

        if (!error)
        {
            startWrite();
        }
        else
        {
            SPDLOG_ERROR("Error while writing {}", error.message());
            disconnect();
        }
    }

    const connection::Options &Tcp::options() const
    {
        return options_;
    }

    const TcpOptions &Tcp::tcpOptions() const
    {
        return options_;
    }

    std::string_view Tcp::type() const
    {
        return kType;
    }
} // namespace connection
