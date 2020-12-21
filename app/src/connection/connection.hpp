#pragma once
#include "connection_handle.hpp"
#include <boost/noncopyable.hpp>
class Connection : public boost::noncopyable
{
public:
    explicit Connection(ConnectionHandle &handle);
    virtual bool isConnected() const = 0;
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual ~Connection();

protected:
    ConnectionHandle &handle_;
};
