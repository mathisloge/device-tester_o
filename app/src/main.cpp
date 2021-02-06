#include <spdlog/spdlog.h>
#include "gui/app.hpp"

#include "protocols/protocol_loader.hpp"
int main(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::trace);

    protocol::ProtocolLoader loader;
    auto protocol = loader.createProtocol("ProtocolNmea");
    if (protocol)
        SPDLOG_INFO("Protocol: {} active={}", protocol->name(), protocol->isActive());
    else
        SPDLOG_WARN("no protocol");

    gui::App app{gui::App::Arguments(argc, argv)};
    return app.exec();
}
