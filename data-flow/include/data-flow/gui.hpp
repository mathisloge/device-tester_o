#pragma once
#include "dataflow_export.h"

#include <imnodes.h>
#include <imgui.h>

namespace dt::df
{
    void DATAFLOW_EXPORT InitDataflow(ImGuiContext *imgui_ctx);
    void DATAFLOW_EXPORT ShutdownDataflow();
} // namespace dt::df
