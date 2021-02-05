#include <boost/dll/alias.hpp>
#include "protocol_nmea.hpp"
namespace protocol
{
    std::unique_ptr<Protocol> createProtocol()
    {
        return std::make_unique<ProtocolNmea>();
    }
} // namespace protocol

BOOST_DLL_ALIAS(protocol::createProtocol, createProtocol)
