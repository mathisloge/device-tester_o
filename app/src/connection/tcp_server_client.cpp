#include "tcp_server_client.hpp"
#include "tcp_server_manager.hpp"
namespace connection
{
    TcpServerClient::TcpServerClient(tcp::socket socket, TcpServerManager &manager, TcpServerConnectionHandle &handler)
        : socket_{std::move(socket)},
          manager_{manager},
          handler_{handler}
    {
    }
    void TcpServerClient::start()
    {
        startRead();
    }
    void TcpServerClient::stop()
    {
        socket_.close();
    }

    void TcpServerClient::shutdown()
    {
        // Initiate graceful connection closure.
        boost::system::error_code ignored_ec;
        socket_.shutdown(tcp::socket::shutdown_both, ignored_ec);
    }

    void TcpServerClient::write(std::span<uint8_t> data)
    {
        auto self(shared_from_this());
        std::shared_ptr<std::vector<uint8_t>> buffer_tx = std::make_shared<std::vector<uint8_t>>(data.begin(), data.end());
        boost::asio::async_write(socket_, boost::asio::buffer(*buffer_tx),
                                 [this, self](boost::system::error_code ec, std::size_t) {
                                     if (ec != boost::asio::error::operation_aborted)
                                     {
                                         manager_.stop(shared_from_this());
                                     }
                                 });
    }

    void TcpServerClient::startRead()
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(rx_buffer_),
                                [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                                    if (!ec)
                                    {
                                        handler_.processData(shared_from_this(), std::span<uint8_t>(rx_buffer_.begin(), rx_buffer_.begin() + bytes_transferred));
                                        startRead();
                                    }
                                    else if (ec != boost::asio::error::operation_aborted)
                                    {
                                        manager_.stop(shared_from_this());
                                    }
                                });
    }

    TcpServerClient::~TcpServerClient()
    {
        shutdown();
        stop();
    }

} // namespace connection
