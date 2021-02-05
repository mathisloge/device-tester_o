#include "protocol_ublox.hpp"
#include <comms/util/detect.h>

namespace protocol
{
    Ublox::Ublox()
        : state_{State::activated}
    {
    }
    std::string_view Ublox::name() const
    {
        return "ublox";
    }

    std::pair<ProtoCIter, ProtoCIter> Ublox::consumeOneMessage(ProtoCIter begin, ProtoCIter end)
    {
        if (!isActive())
            return std::make_pair(begin, begin);

        std::pair<ProtoCIter, ProtoCIter> read_range;
        const auto status = ubx_instance_.processSingle(begin, end, read_range, ubx_handler_);
        return status == comms::ErrorStatus::Success ? read_range : std::make_pair(begin, begin);
    }

    void Ublox::setState(State state)
    {
        state_ = state;
    }
    bool Ublox::isActive() const
    {
        return state_ == State::activated;
    }
} // namespace protocol
