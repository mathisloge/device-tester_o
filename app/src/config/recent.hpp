#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include "../connection/config_io.hpp"
#include "../connection/connection.hpp"

namespace config
{
    struct RecentConnection
    {
        std::string connection_type;
        std::shared_ptr<connection::Options> options;
    };
    std::vector<RecentConnection> recentConnections(const std::string &recent_conns_cfg_file);

    template <typename T>
    bool addRecentConnection(const std::string &recent_conns_cfg_file, const std::string_view type, const T &options)
    {
        nlohmann::json j;
        {
            std::ifstream input_file{recent_conns_cfg_file};
            if (!input_file.is_open())
                SPDLOG_WARN("recent files config file could not be opended. File: {}", recent_conns_cfg_file);
            else
                input_file >> j;
        }

        j["connections"].push_back({{"type", std::string(type)}, {"options", options}});

        {
            std::ofstream o(recent_conns_cfg_file);
            if (!o.is_open())
            {
                SPDLOG_ERROR("recent files config file could not be opended for write. File: {}", recent_conns_cfg_file);
                return false;
            }
            o << j << std::endl;
        }
        return true;
    }
} // namespace config
