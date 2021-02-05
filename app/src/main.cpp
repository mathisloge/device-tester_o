#include <spdlog/spdlog.h>
#include "gui/app.hpp"

#include "protocols/protocol_loader.hpp"
int main(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::trace);

    protocol::ProtocolLoader loader("protocols");
    loader.findAllProtocols();
    return 1;

    gui::App app{gui::App::Arguments(argc, argv)};
    return app.exec();
}
