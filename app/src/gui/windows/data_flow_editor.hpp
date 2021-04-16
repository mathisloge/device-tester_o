#pragma once
#include <vector>
#include <ImNodes.h>
#include "base_window.hpp"
#include "../../protocols/protocol_loader.hpp"
#include <dt/df/editor/editor.hpp>

namespace gui
{
    class DataFlowEditor : public BaseWindow
    {
        static constexpr const char* kDndTarget = "DND_DATAFLOW";
    public:
        explicit DataFlowEditor(const std::string &win_name);
    private:
        void drawContent() override;
        void handleDnD();

    private:
        std::vector<std::string> protocol_names_;
        dt::df::editor::Editor df_editor_;
    };
} // namespace gui
