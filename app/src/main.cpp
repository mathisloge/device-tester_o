#include <spdlog/spdlog.h>
#include "gui/app.hpp"
int main(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::trace);
    gui::App app{gui::App::Arguments(argc, argv)};
    return app.exec();
}
