#include "connection.hpp"

namespace connection
{
    Connection::Connection(ConnectionHandle &handle)
        : handle_{handle}
    {
    }

    const std::string &Connection::identifier() const
    {
        return options().identifier;
    }

    Connection::~Connection()
    {
    }
} // namespace connection
