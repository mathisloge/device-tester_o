#pragma once
#include "base_window.hpp"
namespace gui
{
    class ConnectionWin : public BaseWindow
    {
    public:
        explicit ConnectionWin(const std::string &name);

    protected:
        virtual void drawConnectionOverview() = 0;
        virtual void drawConnectionDetails() = 0;
        virtual void drawConnectionSettings() = 0;
        void drawRawOutput();

    private:
        void drawContent() override;
    };
} // namespace gui
