#pragma once
#include "priv_types.hpp"
#include "data-flow/nodes/node_types.hpp"
#include <unordered_map>
#include <atomic>
namespace dt::df
{
    class GraphImpl
    {
    public:
        void registerNodeFactory(const NodeKey &key, NodeFactory &&factory);
        void createNode(const NodeKey &key);
        void removeNode(const NodeId id);
        void addEdge(const VertexDesc from, const VertexDesc to);
        void removeEdge(const EdgeId id);
        VertexDesc findVertexById(const NodeId id) const;

        void renderNodes();
        void renderLinks();

    private:
        VertexDesc addVertex(const VertexDesc node_desc, const int id, const int parent_id, VertexType type);
        void removeNodeSlots(const Slots &slots);

    private:
        Graph graph_;
        std::atomic_int link_id_counter_;
        std::atomic_int vertex_id_counter_;
        std::unordered_map<NodeKey, NodeFactory> node_factories_;
        std::unordered_map<NodeId, NodePtr> nodes_;
    };
} // namespace dt::df
