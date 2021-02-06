#include "protocol_loader.hpp"
#include <functional>
#include <spdlog/spdlog.h>
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/PluginManager/PluginMetadata.h>
#include <dt-protocol/protocol_plugin.hpp>

using namespace Corrade;
namespace protocol
{
    ProtocolLoader::ProtocolLoader()
    {
    }

    std::vector<std::string> ProtocolLoader::findAllProtocols()
    {
        return manager_.pluginList();
    }

    std::unique_ptr<Protocol> ProtocolLoader::createProtocol(const std::string &protocol_plugin_name)
    {
        SPDLOG_INFO("Loading protocol from plugin {}", protocol_plugin_name);

        if (!(manager_.load(protocol_plugin_name) & PluginManager::LoadState::Loaded))
        {
            SPDLOG_ERROR("The requested plugin {} cannot be loaded.", protocol_plugin_name);
            return nullptr;
        }
        auto protocol_plugin = manager_.instantiate(protocol_plugin_name);

        SPDLOG_INFO("Protocol version: {}", protocol_plugin->protocolVersion());
        return protocol_plugin->createProtocol();
    }

    ProtocolLoader::~ProtocolLoader()
    {
    }

} // namespace protocol
