#include "connection.hpp"

Connection::Connection(ConnectionHandle &handle, const std::string &identifier)
    : handle_{handle}, identifier_{identifier}
{
}

const std::string &Connection::identifier()
{
    return identifier_;
}

Connection::~Connection()
{
}
