#pragma once
#include <memory>
#include <array>
#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include "connection.hpp"

namespace connection
{
    struct SerialOptions : Options
    {
        static constexpr auto kDefaultPar = boost::asio::serial_port_base::parity::none;
        static constexpr auto kDefaultCharSize = 8;
        static constexpr auto kDefaultFlow = boost::asio::serial_port_base::flow_control::none;
        static constexpr auto kDefaultStopBits = boost::asio::serial_port_base::stop_bits::one;

        std::string port;
        unsigned int baud_rate;
        boost::asio::serial_port_base::parity parity;
        boost::asio::serial_port_base::character_size char_size;
        boost::asio::serial_port_base::flow_control flow_control;
        boost::asio::serial_port_base::stop_bits stop_bits;

        SerialOptions();
        SerialOptions(const std::string &identifier);
    };

    class Serial final : public std::enable_shared_from_this<Serial>, public Connection
    {
    public:
        static constexpr std::string_view kType = "serial";

    public:
        explicit Serial(ConnectionHandle &handle, boost::asio::io_context &io_context, const std::string &identifier);
        void applyOptions() override;
        void setOptions(const SerialOptions &options);
        void setOption(const std::string &devname);
        void setOption(unsigned int baud_rate);
        void setOption(const boost::asio::serial_port_base::parity &parity);
        void setOption(const boost::asio::serial_port_base::character_size &char_size);
        void setOption(const boost::asio::serial_port_base::flow_control &flow_control);
        void setOption(const boost::asio::serial_port_base::stop_bits &stop_bits);
        const SerialOptions &serialOptions() const;
        bool isConnected() const override;
        void connect() override;
        void disconnect() override;
        void write(std::span<uint8_t> data) override;
        const connection::Options &options() const override;
        std::string_view type() const override;
        ~Serial();

    private:
        void open();
        void handleRead(const boost::system::error_code &e, std::size_t bytes_transferred);
        void handleWrite(std::shared_ptr<std::vector<uint8_t>> buffer_tx,
                         const boost::system::error_code &error,
                         std::size_t bytes_transferred);
        void processData();

    private:
        /// Strand to ensure the connection's handlers are not called concurrently.
        boost::asio::strand<boost::asio::executor> strand_;
        boost::asio::serial_port serial_;
        std::array<uint8_t, 64> rx_buffer_;
        SerialOptions options_;
    };

} // namespace connection
