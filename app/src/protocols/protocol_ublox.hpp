#pragma once
#include "protocol.hpp"
#include "ublox_detail.hpp"

class ProtocolUblox : public Protocol
{

public:
    explicit ProtocolUblox(detail::proto::UbloxHandler &ubx_handler);
    const char *name() const override;
    std::pair<ProtoCIter, ProtoCIter> consumeOneMessage(ProtoCIter begin, ProtoCIter end) override;

private:
    detail::proto::UbloxInstance ubx_instance_;
    detail::proto::UbloxHandler &ubx_handler_;
};
