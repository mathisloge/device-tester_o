#include "ublox_protocol.hpp"

const char *UbloxProtocol::name() const
{
    return "ublox";
}

bool UbloxProtocol::containsMessage(Data::const_iterator begin, Data::const_iterator end) const
{
    if (!isActive())
        return false;

    return true;
}

std::pair<Protocol::Data::const_iterator, Protocol::Data::const_iterator> UbloxProtocol::consumeOneMessage(Data::const_iterator begin, Data::const_iterator end)
{
    return std::make_pair(begin + 1, end - 1);
}