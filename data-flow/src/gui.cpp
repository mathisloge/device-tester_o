#include "data-flow/gui.hpp"
#include <imnodes.h>

namespace dt::df
{
    void InitDataflow(ImGuiContext *imgui_ctx)
    {
#ifndef DATAFLOW_STATIC_DEFINE
#ifdef WIN32
        ImGui::SetCurrentContext(imgui_ctx);
        imnodes::Initialize();
#endif
#endif
    }

    void DATAFLOW_EXPORT ShutdownDataflow()
    {
#ifndef DATAFLOW_STATIC_DEFINE
#ifdef WIN32
        imnodes::Shutdown();
#endif
#endif
    }
} // namespace dt::df
