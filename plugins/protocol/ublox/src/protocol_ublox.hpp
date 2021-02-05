#pragma once
#include <dt-protocol/protocol.hpp>

#include "ublox_detail.hpp"

namespace protocol
{
    class Ublox : public Protocol
    {
    public:
        Ublox();
        std::string_view name() const override;
        std::pair<ProtoCIter, ProtoCIter> consumeOneMessage(ProtoCIter begin, ProtoCIter end) override;
        void setState(State state) override;
        bool isActive() const override;

    private:
    State state_;
        detail::UbloxInstance ubx_instance_;
        detail::UbloxHandler ubx_handler_;
    };
} // namespace protocol
