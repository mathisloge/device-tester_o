#pragma once
#include "priv_types.hpp"
#include "data-flow/nodes/node_types.hpp"
#include <unordered_map>
#include <atomic>
#include <thread>
#include "bounded_buffer.hpp"
namespace dt::df
{
    class GraphImpl final
    {
    public:
        GraphImpl();
        void registerNodeFactory(const NodeKey &key, NodeFactory &&factory);
        void createNode(const NodeKey &key);
        void removeNode(const NodeId id);
        void addEdge(const VertexDesc from, const VertexDesc to);
        void removeEdge(const EdgeId id);
        VertexDesc findVertexById(const NodeId id) const;

        void renderNodes();
        void renderLinks();
        ~GraphImpl();

    private:
        VertexDesc addVertex(const VertexDesc node_desc, const int id, const int parent_id, VertexType type);
        void removeNodeSlots(const Slots &slots);
        void evaluationTask();
        void reevaluateSlot(SlotId slot);
    private:
        Graph graph_;
        std::atomic_int link_id_counter_;
        std::atomic_int vertex_id_counter_;
        std::unordered_map<NodeKey, NodeFactory> node_factories_;
        std::unordered_map<NodeId, NodePtr> nodes_;


        bool run_evaluation_;
        std::thread evaluation_thread_;
        bounded_buffer<SlotId> evaluation_queue_;
    };
} // namespace dt::df
