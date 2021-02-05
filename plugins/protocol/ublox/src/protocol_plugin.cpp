#include <boost/dll/alias.hpp>
#include "protocol_ublox.hpp"
namespace protocol
{
    std::unique_ptr<Protocol> createProtocol()
    {
        return std::make_unique<Ublox>();
    }
} // namespace protocol

BOOST_DLL_ALIAS(protocol::createProtocol, createProtocol)
