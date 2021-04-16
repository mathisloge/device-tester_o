#if 0
#include <catch.hpp>
#include <iostream>
#include <span>
#include <dt-protocol/protocol.hpp>
#include "protocols/dynamic_protocol_dispatcher.hpp"

using namespace protocol;
class MockProtocolCorrect : public Protocol
{
public:
    const int &offset_begin_;
    const int &neg_offset_end_;
    std::vector<uint8_t> message_;
    MockProtocolCorrect(const int &offset_begin, const int &neg_offset_end)
        : offset_begin_{offset_begin}, neg_offset_end_{neg_offset_end} {}
    std::string_view name() const override
    {
        return "MockProtocolCorrect";
    }

    std::pair<ProtoCIter, ProtoCIter> consumeOneMessage(ProtoCIter begin, ProtoCIter end) override
    {
        message_.insert(message_.end(), begin + offset_begin_, end - neg_offset_end_);
        return std::make_pair(begin + offset_begin_, end - neg_offset_end_);
    }
    void setState(State) override {}
    bool isActive() const override { return true; }
};
class MockProtocolFails : public Protocol
{
};

SCENARIO("protocol dispatcher with simple protocols", "[protocol-dispatcher]")
{
    GIVEN("A dispatcher with one protocol")
    {
        std::vector<uint8_t> message{0, 1, 2, 3, 4};
        int start_offset = 0;
        int end_offset = 0;

        DynamicProtocolDispatcher dispatcher;
        dispatcher.addProtocol(std::make_unique<MockProtocolCorrect>(start_offset, end_offset));
        WHEN("data is added")
        {
            dispatcher.appendData(message);
            THEN("protocol should have a message and size 0")
            {
                const auto &resv_msg = static_cast<MockProtocolCorrect&>(*dispatcher.protocols()[0]).message_;
                REQUIRE(message == resv_msg);
                REQUIRE(dispatcher.data().size() == 0);
            }
        }
        WHEN("data is added and the first bytes are not needed")
        {
            start_offset = 2;
            dispatcher.appendData(message);
            THEN("protocol should have a message and size 0")
            {
                const auto &resv_msg = static_cast<MockProtocolCorrect&>(*dispatcher.protocols()[0]).message_;
                // we create a new vector here to see the differences of the vector if the test fails. std::equal is not verbose.
                REQUIRE(std::vector<uint8_t>{message.begin() + start_offset, message.end()} == resv_msg);
                REQUIRE(dispatcher.data().size() == 0);
            }
        }
        WHEN("data is not completly used")
        {
            end_offset = 2;
            dispatcher.appendData(message);
            THEN("protocol should have a message and bytes should left")
            {
                const auto &resv_msg = static_cast<MockProtocolCorrect&>(*dispatcher.protocols()[0]).message_;
                REQUIRE(std::vector<uint8_t>{message.begin(), message.end() - end_offset} == resv_msg);
                REQUIRE(dispatcher.data().size() == end_offset);
            }
        }
    }
}
#endif
