#include "recent.hpp"
#include "../connection/config_io.hpp"
#include "../connection/serial_connection.hpp"
#include "../connection/tcp_connection.hpp"
#include "../connection/udp_connection.hpp"
using json = nlohmann::json;

namespace con = connection;
namespace config
{

    std::vector<RecentConnection> recentConnections(const std::string &recent_conns_cfg_file)
    {
        std::vector<RecentConnection> recent_connections;

        std::ifstream input_file{recent_conns_cfg_file};
        if (!input_file.is_open())
        {
            SPDLOG_WARN("recent files config file could not be opended. File: {}", recent_conns_cfg_file);
            return recent_connections;
        }

        json j;
        input_file >> j;

        if (j.contains("connections") && j["connections"].is_array())
        {
            const auto connections = j["connections"];
            for (const auto &con : connections)
            {
                SPDLOG_DEBUG("Found {} and has options: {}", con["type"].get<std::string>(), con.contains("options"));
                RecentConnection recent_con;
                recent_con.connection_type = con["type"].get<std::string>();
                if (recent_con.connection_type == con::Serial::kType && con.contains("options"))
                    recent_con.options = std::make_shared<con::SerialOptions>(con["options"].get<con::SerialOptions>());
                else if (recent_con.connection_type == con::Tcp::kType && con.contains("options"))
                    recent_con.options = std::make_shared<con::TcpOptions>(con["options"].get<con::TcpOptions>());
                else if (recent_con.connection_type == con::Udp::kType && con.contains("options"))
                    recent_con.options = std::make_shared<con::UdpOptions>(con["options"].get<con::UdpOptions>());
                else
                {
                    SPDLOG_WARN("Unknown connection type: {}", recent_con.connection_type);
                    continue;
                }
                recent_connections.emplace_back(std::move(recent_con));
            }
        }
        return recent_connections;
    }
} // namespace config
