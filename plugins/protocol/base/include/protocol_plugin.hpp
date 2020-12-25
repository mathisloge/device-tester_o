#pragma once
#include <cstdint>
#include <vector>
#include <boost/config.hpp>
#include <boost/noncopyable.hpp>

namespace protocol
{
    enum class State
    {
        activated,
        deactivated
    };
    using ProtoData = std::vector<uint8_t>;
    using ProtoCIter = ProtoData::const_iterator;

    class BOOST_SYMBOL_VISIBLE ProtocolPlugin : public boost::noncopyable
    {
    public:
        virtual void init() = 0;
        virtual void deinit() = 0;
        virtual void setState(State state) = 0;
        virtual bool isActive() const = 0;
        virtual std::string name() const = 0;

        virtual std::pair<ProtoCIter, ProtoCIter> consumeOneMessage(ProtoCIter begin, ProtoCIter end) = 0;
        virtual ~ProtocolPlugin() {}
    };
} // namespace protocol
