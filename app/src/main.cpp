#include <iostream>
#include <array>
#include "protocols/protocol_dispatcher.hpp"
#include "protocols/ublox_protocol.hpp"
int main(int argc, char const *argv[])
{
    spdlog::set_level(spdlog::level::trace);
    
    std::vector<uint8_t> d;
    UbloxProtocol{}.containsMessage(d.begin(), d.end());
    ProtocolDispatcher<UbloxProtocol> test{UbloxProtocol{}};
    std::array<uint8_t, 10> testdata;
    for (int i = 0; i < testdata.size(); i++)
        testdata[i] = i;
    test.appendData(&testdata[0], testdata.size());
    std::cout << "test" << std::endl;
    return 0;
}
