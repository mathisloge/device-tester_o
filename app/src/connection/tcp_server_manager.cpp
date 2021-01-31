#include "tcp_server_manager.hpp"

namespace connection
{

    TcpServerManager::TcpServerManager()
    {
    }

    void TcpServerManager::start(TcpServerClientPtr client)
    {
        connections_.insert(client);
        client->start();
    }

    void TcpServerManager::stop(TcpServerClientPtr client)
    {
        connections_.erase(client);
        client->stop();
    }

    void TcpServerManager::stopAll()
    {
        for (auto c : connections_)
            c->stop();
        connections_.clear();
    }

    TcpServerManager::~TcpServerManager()
    {
        stopAll();
    }
} // namespace connection
