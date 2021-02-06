#include "protocol_loader.hpp"
#include <functional>
#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>
#include <dt-protocol/protocol_plugin.hpp>
#include <spdlog/spdlog.h>
#include <Corrade/PluginManager/Manager.h>

namespace dll = boost::dll;
namespace fs = boost::filesystem;
using namespace Corrade;
namespace protocol
{
    ProtocolLoader::ProtocolLoader()
    {
    }

    void ProtocolLoader::findAllProtocols()
    {
        
    }

    // todo: wrap protocol in a own FactoryInstance so we can hold the protocol library in memory.

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
