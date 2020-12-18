#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include "connection/serial_connection.hpp"
#include "gnss/ublox_device.hpp"
#include "protocols/protocol_ublox.hpp"
#include "protocols/protocol_nmea.hpp"
class MsgHandler : public UbloxHandler, public NmeaHandler
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

int main(int argc, char const *argv[])
{
    spdlog::set_level(spdlog::level::trace);

    MsgHandler msg_handler;
    UbloxDevice ublox_device{msg_handler, msg_handler};
    boost::asio::io_context io;
    std::shared_ptr<SerialConnection> con = std::make_shared<SerialConnection>(ublox_device, io);
    con->open("COM8", 115200);

    io.run();
    return 0;
}
