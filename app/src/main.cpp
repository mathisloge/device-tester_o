#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include "connection/serial_connection.hpp"
#include "protocols/protocol_dispatcher.hpp"
#include "protocols/protocol_ublox.hpp"

class MsgHandler : public detail::proto::UbloxHandler
{
public:
    void handle(detail::proto::UbloxMessage &msg) override
    {

        std::cout << "received ublox message len:" << msg.length() << " " << (msg.hasName() ? msg.name() : "unknown") << std::endl;
    }
};
class Handle : public ConnectionHandle
{
    MsgHandler ublox_handler_;
    ProtocolDispatcher<ProtocolUblox> dispatcher_;

public:
    Handle() : dispatcher_{ProtocolUblox{ublox_handler_}}
    {
    }

    void processData(const uint8_t *data, const size_t len) override
    {
        dispatcher_.appendData(data, len);
    }

    void connectionClosed() override
    {
        std::cout << "connection closed" << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    spdlog::set_level(spdlog::level::trace);

    Handle handle;
    boost::asio::io_context io;
    std::shared_ptr<SerialConnection> con = std::make_shared<SerialConnection>(handle, io);
    con->open("COM8", 115200);

    io.run();
    return 0;
}
