#include <spdlog/spdlog.h>
#include "gui/app.hpp"

#include "protocols/protocol_loader.hpp"
int main(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::trace);

    protocol::ProtocolLoader loader;
    loader.findAllProtocols();
    for (const auto &protocol_name : loader.findAllProtocols())
    {
        auto protocol = loader.createProtocol(protocol_name);
        if (protocol)
            SPDLOG_INFO("Protocol: {} active={}", protocol->name(), protocol->isActive());
        else
            SPDLOG_WARN("no protocol");
    }

    gui::App app{gui::App::Arguments(argc, argv)};
    return app.exec();
}
