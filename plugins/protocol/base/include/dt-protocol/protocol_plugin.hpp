#pragma once
#include <string_view>
#include <string>
#include <memory>
#include <Corrade/PluginManager/AbstractPlugin.h>
#include "protocol.hpp"
namespace protocol
{
    class ProtocolPlugin : public Corrade::PluginManager::AbstractPlugin
    {
    public:
        static std::string pluginInterface()
        {
            return "de.mathisloge.devicetester.ProtocolPlugin/1.0";
        }

        static std::vector<std::string> pluginSearchPaths()
        {
            return {"C:/dev/install/device_tester/bin/protocols/dbg"};
        }

        explicit ProtocolPlugin(Corrade::PluginManager::AbstractManager &manager, const std::string &plugin)
            : AbstractPlugin{manager, plugin} {}
        virtual std::string_view protocolVersion() const = 0;
        virtual std::unique_ptr<Protocol> createProtocol() const = 0;
        virtual ~ProtocolPlugin() = default;
    };
} // namespace protocol
