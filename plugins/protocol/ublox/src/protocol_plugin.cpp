#include <Corrade/PluginManager/AbstractManager.h>
#include <dt-protocol/protocol_plugin.hpp>
#include "protocol_ublox.hpp"
namespace protocol
{
    class UbloxProtocolPlugin final : public ProtocolPlugin
    {

    public:
        explicit UbloxProtocolPlugin(Corrade::PluginManager::AbstractManager &manager, const std::string &plugin)
            : ProtocolPlugin{manager, plugin} {}

        std::string_view protocolVersion() const override
        {
            return "UBX9";
        }
        std::unique_ptr<Protocol> createProtocol() const override
        {
            return std::make_unique<Ublox>();
        }
    };
} // namespace protocol

CORRADE_PLUGIN_REGISTER(ProtocolUblox, protocol::UbloxProtocolPlugin, "de.mathisloge.devicetester.ProtocolPlugin/1.0")
