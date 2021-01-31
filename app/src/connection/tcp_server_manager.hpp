#pragma once
#include <set>
#include <boost/noncopyable.hpp>
#include "tcp_server_client.hpp"
namespace connection
{
    class TcpServerManager final : public boost::noncopyable
    {
    public:
        TcpServerManager();
        void start(TcpServerClientPtr client);
        void stop(TcpServerClientPtr client);
        void stopAll();
        ~TcpServerManager();

    private:
        std::set<TcpServerClientPtr> connections_;
    };
} // namespace connection
