#pragma once
#include "data_flow_graph.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT DataFlowEditor
    {
    public:
        static constexpr std::string_view kDndTarget = "DND_DATAFLOW";

    public:
        DataFlowEditor();
        DataFlowEditor(const DataFlowEditor &) = delete;
        DataFlowEditor &operator=(const DataFlowEditor &) = delete;

        void render();
        void renderNodeDisplayTree(const NodeDisplayDrawFnc &draw_fnc) const;
        DataFlowGraph &graph();
        const DataFlowGraph &graph() const;

        virtual ~DataFlowEditor();

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
