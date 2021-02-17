#pragma once
#include <functional>
#include <filesystem>
#include "types.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    class GraphImpl;
    class DATAFLOW_EXPORT DataFlowGraph
    {
    public:
        DataFlowGraph();
        DataFlowGraph(const DataFlowGraph &) = delete;
        DataFlowGraph &operator=(const DataFlowGraph &) = delete;

        void registerBuildinNodes();
        void registerNode(const NodeKey &key, const std::string &node_display_name, NodeFactory &&factory, NodeDeserializationFactory &&deser_factory);
        void addNode(const NodeKey &key, int preferred_x = 0, int preferred_y = 0, bool screen_space = false);
        void removeNode(const NodeId id);
        void addEdge(const NodeId from, const NodeId to);
        void removeEdge(const EdgeId id);

        void render();
        void renderNodeDisplayTree(const NodeDisplayDrawFnc &draw_fnc) const;
        void save(const std::filesystem::path &file);
        void clear();
        void clearAndLoad(const std::filesystem::path &file);

        virtual ~DataFlowGraph();

    private:
        GraphImpl *impl_;
        friend GraphImpl;
    };
} // namespace dt::df
