#pragma once
#include <string>
#include <boost/noncopyable.hpp>
#include "connection_handle.hpp"
#include "connection_options.hpp"

namespace connection
{
    class Connection : public boost::noncopyable
    {
    public:
        explicit Connection(ConnectionHandle &handle);
        virtual bool isConnected() const = 0;
        virtual void connect() = 0;
        virtual void disconnect() = 0;
        virtual void write(std::span<uint8_t> data) = 0;
        virtual void applyOptions() = 0;
        virtual const Options &options() const = 0;
        const std::string &identifier() const;
        /**
     * @brief Function that outputs the type of the connection. Have to be unique for each connection type.
     * Eg. tcp for TCP, udp for UCP, serial for Serial etc.
     * 
     * @return std::string_view
     */
        virtual std::string_view type() const = 0;
        virtual ~Connection();

    protected:
        ConnectionHandle &handle_;
    };

} // namespace connection
