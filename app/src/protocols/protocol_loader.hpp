#pragma once
#include <filesystem>
#include <set>
#include <map>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <dt-protocol/protocol.hpp>
namespace protocol
{
    class ProtocolLoader : public boost::noncopyable
    {
    public:
        explicit ProtocolLoader(const boost::filesystem::path &initial_search_path);

        void findAllProtocols();
        std::unique_ptr<Protocol> createProtocol(const std::string &protocol_plugin_name);
        ~ProtocolLoader();

    private:
        boost::filesystem::path getPlugin(const std::string &protocol_plugin_name) const;

    private:
        std::set<boost::filesystem::path> plugin_search_paths_;
    };
} // namespace protocol
