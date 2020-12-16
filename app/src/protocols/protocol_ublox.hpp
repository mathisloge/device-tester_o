#pragma once
#include "protocol.hpp"

class ProtocolUblox : public Protocol
{
public:
    const char *name() const override;
    bool containsMessage(Data::const_iterator begin, Data::const_iterator end) const override;
    std::pair<Data::const_iterator, Data::const_iterator> consumeOneMessage(Data::const_iterator begin, Data::const_iterator end) override;
};