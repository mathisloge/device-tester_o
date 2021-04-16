#pragma once
#if 0
#include <boost/noncopyable.hpp>
#include <Corrade/PluginManager/Manager.h>
#include <dt-protocol/protocol_plugin.hpp>
#include <unordered_map>
namespace protocol
{
    class ProtocolLoader : public boost::noncopyable
    {
    public:
        ProtocolLoader();
        std::vector<std::string> findAllProtocols();
        std::unique_ptr<Protocol> createProtocol(const std::string &protocol_plugin_name);
        ~ProtocolLoader();

    private:
        Corrade::PluginManager::Manager<ProtocolPlugin> manager_;
    };
} // namespace protocol
#endif
