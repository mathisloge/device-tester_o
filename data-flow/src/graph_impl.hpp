#pragma once
#include "data-flow/types.hpp"
#include <unordered_map>
#include <atomic>
namespace dt::df
{
    class GraphImpl
    {
    public:
        void addEdge(const VertexDesc from, const VertexDesc to);
        VertexDesc findVertexById(const NodeId id) const;

    private:
        Graph graph_;
        std::atomic_int link_id_counter_;
        std::atomic_int vertex_id_counter_;
        std::unordered_map<NodeId, NodePtr> nodes_;
    };
} // namespace dt::df
