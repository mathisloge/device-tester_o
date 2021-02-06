#pragma once
#include "base_window.hpp"
#include <ImNodes.h>
namespace gui
{
    class DataFlowEditor : public BaseWindow
    {
    public:
        explicit DataFlowEditor(const std::string &win_name);

    private:
        void drawContent() override;
        void handleDnD();
    private:
        ImNodes::CanvasState canvas_;
    };
} // namespace gui
