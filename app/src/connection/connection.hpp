#pragma once
#include <string>
#include <boost/noncopyable.hpp>
#include "connection_handle.hpp"
class Connection : public boost::noncopyable
{
public:
    explicit Connection(ConnectionHandle &handle, const std::string &identifier);
    virtual bool isConnected() const = 0;
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    const std::string &identifier();
    virtual ~Connection();

protected:
    ConnectionHandle &handle_;
    const std::string identifier_;
};
