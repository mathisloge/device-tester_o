#include "protocol_loader.hpp"
#include <functional>
#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>
#include <dt-protocol/protocol.hpp>
#include <spdlog/spdlog.h>

namespace dll = boost::dll;
namespace fs = boost::filesystem;

namespace protocol
{
    ProtocolLoader::ProtocolLoader(const boost::filesystem::path &initial_search_path)
        : plugin_search_paths_{initial_search_path}
    {
    }

    void ProtocolLoader::findAllProtocols()
    {
        for (const auto &search_path : plugin_search_paths_)
        {
            for (const auto &file : fs::directory_iterator(search_path))
            {
                if (fs::is_regular_file(file))
                {
                    SPDLOG_INFO("Found plugin: {}", file.path().stem().string());
                    createProtocol(file.path().stem().string());
                }
            }
        }
    }

    // todo: wrap protocol in a own FactoryInstance so we can hold the protocol library in memory.

    std::unique_ptr<Protocol> ProtocolLoader::createProtocol(const std::string &protocol_plugin_name)
    {
        const auto plugin_path = getPlugin(protocol_plugin_name);
        if (!fs::exists(plugin_path))
            return nullptr;

        SPDLOG_INFO("Loading protocol from plugin {}", plugin_path.string());

        typedef std::unique_ptr<Protocol>(plugin_protocol_create_t)();
        std::function<plugin_protocol_create_t> creator;
        try
        {
            creator = boost::dll::import_alias<plugin_protocol_create_t>(
                plugin_path,
                "createProtocol",
                dll::load_mode::default_mode);

            std::unique_ptr<Protocol> protocol = creator();
            SPDLOG_INFO("Protocol name: {}", protocol->name());
        }
        catch (std::exception ex)
        {
            SPDLOG_ERROR("Error while loading protocol: {}", ex.what());
        }
    }

    fs::path ProtocolLoader::getPlugin(const std::string &protocol_plugin_name) const
    {
        for (const auto &p_path : plugin_search_paths_)
        {
            const auto plugin_path = dll::shared_library::decorate(p_path / protocol_plugin_name);
            if (boost::filesystem::exists(plugin_path))
                return plugin_path;
        }
        return fs::path();
    }

    ProtocolLoader::~ProtocolLoader()
    {
    }

} // namespace protocol
