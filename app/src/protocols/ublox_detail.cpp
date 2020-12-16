#include "ublox_detail.hpp"
#include <comms/comms.h>
#include <comms/process.h>

namespace detail::proto
{
    void UbloxHandler::handle(UbloxMessage &msg)
    {
    }

    comms::ErrorStatus UbloxInstance::processSingle(ProtoCIter begin, ProtoCIter end,
                                                    std::pair<ProtoCIter, ProtoCIter> &read_range,
                                                    UbloxHandler &handler)
    {
        using FrameType = typename std::decay<decltype(frame)>::type;
        using MsgPtr = typename FrameType::MsgPtr;
        MsgPtr msg;
        ProtoCIter read_it = begin;
        const auto status = comms::processSingleWithDispatch(read_it, std::distance(begin, end), frame, msg, handler);

        read_range.first = begin;
        read_range.second = read_it;
        return status;
    }
} // namespace detail::proto