#include "tcp_server.hpp"

namespace connection
{

    TcpServer::TcpServer(TcpServerConnectionHandle &handle, boost::asio::io_context &io_context, const std::string &identifier)
        : Connection(handle),
          strand_{boost::asio::make_strand(io_context)},
          acceptor_(strand_),
          conn_handle_{handle},
          options_{identifier}
    {
    }

    bool TcpServer::isConnected() const
    {
        return acceptor_.is_open();
    }

    void TcpServer::connect()
    {
        disconnect();

        acceptor_ = tcp::acceptor(strand_, tcp::endpoint(options_.protocol == TcpServerOptions::Protocol::ipv4 ? tcp::v4() : tcp::v6(), options_.port));
    }

    void TcpServer::disconnect()
    {
        manager_.stopAll();
        acceptor_.close();
    }

    void TcpServer::startAccept()
    {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                if (!acceptor_.is_open())
                {
                    return;
                }

                if (!ec)
                {
                    manager_.start(std::make_shared<TcpServerClient>(std::move(socket), manager_, conn_handle_));
                }
                startAccept();
            });
    }

    void TcpServer::write(std::span<uint8_t> data)
    {
    }

    void TcpServer::applyOptions()
    {
        if (acceptor_.is_open())
        {
            connect();
        }
    }

    void TcpServer::setOptions(const TcpServerOptions &options)
    {
        options_ = options;
    }

    const Options &TcpServer::options() const
    {
        return options_;
    }

    std::string_view TcpServer::type() const
    {
        return kType;
    }

    TcpServer::~TcpServer()
    {
        disconnect();
    }
} // namespace connection
