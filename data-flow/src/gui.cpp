#include "data-flow/gui.hpp"

namespace dt::df
{
    void InitDataflow(ImGuiContext *imgui_ctx)
    {
        ImGui::SetCurrentContext(imgui_ctx);
    }
} // namespace dt::df
