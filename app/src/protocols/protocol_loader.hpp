#pragma once
#include <boost/noncopyable.hpp>
#include <Corrade/PluginManager/Manager.h>
#include <dt-protocol/protocol_plugin.hpp>
namespace protocol
{
    class ProtocolLoader : public boost::noncopyable
    {
    public:
        ProtocolLoader();
        void findAllProtocols();
        std::unique_ptr<Protocol> createProtocol(const std::string &protocol_plugin_name);
        ~ProtocolLoader();

    private:
        Corrade::PluginManager::Manager<ProtocolPlugin> manager_;
    };
} // namespace protocol
