#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include "connection/serial_connection.hpp"
#include "protocols/protocol_dispatcher.hpp"
#include "protocols/protocol_ublox.hpp"
#include "protocols/protocol_nmea.hpp"
class MsgHandler : public detail::proto::UbloxHandler, public NmeaHandler
{
public:
    void handle(detail::proto::UbloxMessage &msg) override
    {

        std::cout << "received ublox message len:" << msg.length() << " " << (msg.hasName() ? msg.name() : "unknown") << std::endl;
    }
    void handle(const NmeaMessage &msg) override
    {
        spdlog::critical("got completly unkown message. it is not unsupported.");
    }
    void handle(const NmeaDTM &msg) override
    {
        std::cout << "got dtm nmea message " << std::endl;
    }
    void handle(const NmeaGLL &msg) override
    {
        spdlog::info("my position: https://www.google.com/maps/dir/{},{}", msg.latitude, msg.longitude);
    }
    void handle(const NmeaUnsupported &msg) override
    {
        //std::cout << "got unkown nmea message " << std::endl;
    }
};
class Handle : public ConnectionHandle
{
    MsgHandler msg_handler_;
    ProtocolDispatcher<ProtocolUblox, ProtocolNmea> dispatcher_;

public:
    Handle() : dispatcher_{ProtocolUblox{msg_handler_}, ProtocolNmea{msg_handler_}}
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
