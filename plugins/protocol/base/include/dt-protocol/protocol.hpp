#pragma once
#include <string_view>
#include <boost/config.hpp>
#include <boost/noncopyable.hpp>
#include "protocol_types.hpp"
namespace protocol
{
    class BOOST_SYMBOL_VISIBLE Protocol : public boost::noncopyable
    {
    public:
        enum class State
        {
            activated,
            deactivated
        };

    public:
        Protocol() = default;
        virtual std::string_view name() const = 0;
        virtual std::pair<ProtoCIter, ProtoCIter> consumeOneMessage(ProtoCIter begin, ProtoCIter end) = 0;
        virtual void setState(State state) = 0;
        virtual bool isActive() const = 0;
        virtual ~Protocol() = default;
    };
} // namespace protocol
