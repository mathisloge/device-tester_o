#pragma once
#include "connection_handle.hpp"
#include <boost/noncopyable.hpp>
class Connection : public boost::noncopyable
{
public:
    explicit Connection(ConnectionHandle &handle);
    virtual ~Connection();
protected:
    ConnectionHandle &handle_;
};
