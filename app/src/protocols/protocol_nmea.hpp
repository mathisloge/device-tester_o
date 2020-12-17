#pragma once
#include "protocol.hpp"
#include <nmea/nmea_messages.hpp>

class NmeaHandler
{
public:
    virtual void handle(const NmeaMessage &msg) = 0;
    virtual void handle(const NmeaUnsupported &msg) = 0;
    virtual void handle(const NmeaDTM &msg) = 0;
    virtual void handle(const NmeaGLL &msg) = 0;
};
class ProtocolNmea : public Protocol
{

public:
    explicit ProtocolNmea(NmeaHandler &handler);
    const char *name() const override;
    std::pair<ProtoCIter, ProtoCIter> consumeOneMessage(ProtoCIter begin, ProtoCIter end) override;

private:
    NmeaHandler &handler_;
};
