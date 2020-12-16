#include <catch.hpp>
#include <iostream>
#include "protocols/protocol_dispatcher.hpp"
#include "protocols/protocol.hpp"

class MockProtocolCorrect : public Protocol
{
public:
    const int &offset_begin_;
    const int &neg_offset_end_;
    std::vector<uint8_t> message_;
    MockProtocolCorrect(const int &offset_begin, const int &neg_offset_end)
        : offset_begin_{offset_begin}, neg_offset_end_{neg_offset_end} {}
    const char *name() const override
    {
        return "MockProtocolCorrect";
    }
    bool containsMessage(Data::const_iterator begin, Data::const_iterator end) const override
    {
        return true;
    }
    std::pair<Data::const_iterator, Data::const_iterator> consumeOneMessage(Data::const_iterator begin, Data::const_iterator end) override
    {
        message_.insert(message_.end(), begin + offset_begin_, end - neg_offset_end_);
        return std::make_pair(begin + offset_begin_, end - neg_offset_end_);
    }
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
        ProtocolDispatcher<MockProtocolCorrect> dispatcher{MockProtocolCorrect{start_offset, end_offset}};
        WHEN("data is added")
        {
            dispatcher.appendData(&message[0], message.size());
            THEN("protocol should have a message and size 0")
            {
                const auto &resv_msg = std::get<0>(dispatcher.protocols()).message_;
                REQUIRE(message == resv_msg);
                REQUIRE(dispatcher.data().size() == 0);
            }
        }
        WHEN("data is added and the first bytes are not needed")
        {
            start_offset = 2;
            dispatcher.appendData(&message[0], message.size());
            THEN("protocol should have a message and size 0")
            {
                const auto &resv_msg = std::get<0>(dispatcher.protocols()).message_;
                // we create a new vector here to see the differences of the vector if the test fails. std::equal is not verbose.
                REQUIRE(std::vector<uint8_t>{message.begin() + start_offset, message.end()} == resv_msg);
                REQUIRE(dispatcher.data().size() == 0);
            }
        }
        WHEN("data is not completly used")
        {
            end_offset = 2;
            dispatcher.appendData(&message[0], message.size());
            THEN("protocol should have a message and bytes should left")
            {
                const auto &resv_msg = std::get<0>(dispatcher.protocols()).message_;
                REQUIRE(std::vector<uint8_t>{message.begin(), message.end() - end_offset} == resv_msg);
                REQUIRE(dispatcher.data().size() == end_offset);
            }
        }
    }
}
