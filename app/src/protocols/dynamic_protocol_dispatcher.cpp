#include "dynamic_protocol_dispatcher.hpp"
#include <spdlog/spdlog.h>

namespace protocol
{

    DynamicProtocolDispatcher::DynamicProtocolDispatcher()
    {
    }

    void DynamicProtocolDispatcher::addProtocol(std::unique_ptr<Protocol> protocol)
    {
        protocols_.push_back(std::move(protocol));
    }

    void DynamicProtocolDispatcher::appendData(std::span<uint8_t> data)
    {
        data_.reserve(data.size());
        data_.insert(data_.end(), data.begin(), data.end());
        processAllData();
    }

    const std::vector<std::unique_ptr<Protocol>> &DynamicProtocolDispatcher::protocols() const
    {
        return protocols_;
    }

    const ProtoData &DynamicProtocolDispatcher::data() const
    {
        return data_;
    }

    void DynamicProtocolDispatcher::processAllData()
    {
        //! \todo wrap in while. condition => at least one protocol detects a message
        ProtoCIter prev_begin = data_.begin();
        ProtoCIter prev_end = data_.end();
        while (!data_.empty())
        {
            for (auto &protocol : protocols_)
            {
                processProtocol(*protocol);
            }
            if (prev_begin == data_.begin() && prev_end == data_.end())
                return;
            prev_begin = data_.begin();
            prev_end = data_.end();
        }
    }

    void DynamicProtocolDispatcher::processProtocol(Protocol &protocol)
    {
        if (!protocol.isActive())
            return;
        if (auto consumed = protocol.consumeOneMessage(data_.begin(), data_.end()); consumed.second != data_.begin())
        {
            SPDLOG_TRACE("{}: consumed {}", protocol.name(), std::distance(consumed.first, consumed.second));
            if (data_.begin() != consumed.first)
            {
                SPDLOG_DEBUG("dumping first {} byte of buffer", std::distance(data_.cbegin(), consumed.first));
            }

            data_.erase(data_.begin(), consumed.second);
        }
    }

    DynamicProtocolDispatcher::~DynamicProtocolDispatcher()
    {
    }

} // namespace protocol
