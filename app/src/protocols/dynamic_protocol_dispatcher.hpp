#pragma once
#include <memory>
#include <vector>
#include <span>
#include <dt-protocol/protocol.hpp>
namespace protocol
{
    class DynamicProtocolDispatcher final
    {
    public:
        DynamicProtocolDispatcher();
        void addProtocol(std::unique_ptr<Protocol> protocol);
        void appendData(std::span<uint8_t> data);
        const std::vector<std::unique_ptr<Protocol>> &protocols() const;
        const ProtoData &data() const;
        ~DynamicProtocolDispatcher();

    private:
        void processAllData();
        void processProtocol(Protocol &protocol);

    private:
        std::vector<std::unique_ptr<Protocol>> protocols_;
        ProtoData data_;
    };
} // namespace protocol
