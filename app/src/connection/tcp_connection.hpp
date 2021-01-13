#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "connection.hpp"
class TcpConnection : public std::enable_shared_from_this<TcpConnection>, public Connection
{
    using tcp = boost::asio::ip::tcp;

public:
    explicit TcpConnection(ConnectionHandle &handle, const std::string &identifier, boost::asio::io_context &io_context);
    bool isConnected() const override;
    void connect() override;
    void disconnect() override;
    void setOption(const std::string &server, const unsigned short server_port, const std::string &service = "");
    void setOption(char packet_end);

private:
    void beginConnect(tcp::resolver::results_type::iterator endpoint_iter);
    void handleConnect(const boost::system::error_code &error, tcp::resolver::results_type::iterator endpoint_iter);
    void startRead();
    void startWrite();
    void handleRead(const boost::system::error_code &error, std::size_t n);
    void handleWrite(const boost::system::error_code &error);

private:
    /// Strand to ensure the connection's handlers are not called concurrently.
    boost::asio::strand<boost::asio::executor> strand_;
    std::vector<uint8_t> buffer_rx_;
    std::string server_;
    unsigned short server_port_;
    std::string service_;

    char packet_end_;

    tcp::socket socket_;
    tcp::resolver::results_type endpoints_;
    bool should_run_;
};
