#include "connection.hpp"

Connection::Connection(ConnectionHandle &handle)
    : handle_{handle}
{
}

Connection::~Connection()
{
}
