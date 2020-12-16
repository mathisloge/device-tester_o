#pragma once
#include <type_traits>
#include <cstdint>
#include <vector>
#include <tuple>
#include <algorithm>
#include <spdlog/spdlog.h>
#include "protocol.hpp"

template <typename T>
concept IsProtocol = std::is_base_of<Protocol, T>::value;

template <IsProtocol... Ts>
class ProtocolDispatcher
{
public:
    explicit ProtocolDispatcher(Ts... ts) : protocols_{ts...} {}

    void appendData(const uint8_t *data, const size_t len)
    {
        data_.reserve(data_.size() + len);
        data_.insert(data_.end(), data, data + len);
        processAllData();
    }

    const std::tuple<Ts...> &protocols() const { return protocols_; }
    const std::vector<uint8_t> &data() const { return data_; }

private:
    void processAllData()
    {
        //! \todo wrap in while. condition => at least one protocol detects a message
        std::apply([this](auto &... protocol) { (processProtocol(protocol), ...); }, protocols_);
    }

    template <IsProtocol T>
    void processProtocol(T &protocol)
    {
        if (protocol.containsMessage(data_.begin(), data_.end()))
        {
            auto consumed = protocol.consumeOneMessage(data_.begin(), data_.end());

            SPDLOG_TRACE("{}: consumed {}", protocol.name(), std::distance(consumed.first, consumed.second));
            if (data_.begin() != consumed.first)
            {
                SPDLOG_DEBUG("dumping first {} byte of buffer", std::distance(data_.cbegin(), consumed.first));
            }

            data_.erase(data_.begin(), consumed.second);
        }
    }

private:
    std::tuple<Ts...> protocols_;
    std::vector<uint8_t> data_;
};