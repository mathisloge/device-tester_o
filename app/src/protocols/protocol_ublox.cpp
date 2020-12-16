#include "protocol_ublox.hpp"

const char *ProtocolUblox::name() const
{
    return "ublox";
}

bool ProtocolUblox::containsMessage(Data::const_iterator begin, Data::const_iterator end) const
{
    if (!isActive())
        return false;

    return true;
}

std::pair<Protocol::Data::const_iterator, Protocol::Data::const_iterator> ProtocolUblox::consumeOneMessage(Data::const_iterator begin, Data::const_iterator end)
{
    return std::make_pair(begin + 1, end - 1);
}