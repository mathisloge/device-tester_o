#include <Corrade/PluginManager/AbstractManager.h>
#include <dt-protocol/protocol_plugin.hpp>
#include "protocol_nmea.hpp"
namespace protocol
{
    class NmeaProtocolPlugin final : public ProtocolPlugin
    {

    public:
        explicit NmeaProtocolPlugin(Corrade::PluginManager::AbstractManager &manager, const std::string &plugin)
            : ProtocolPlugin{manager, plugin} {}

        std::string_view protocolVersion() const override
        {
            return "NMEA0183";
        }
        std::unique_ptr<Protocol> createProtocol() const override
        {
            return std::make_unique<ProtocolNmea>();
        }
    };
} // namespace protocol

CORRADE_PLUGIN_REGISTER(NmeaProtocol, protocol::NmeaProtocolPlugin, "de.mathisloge.devicetester.ProtocolPlugin/1.0")
