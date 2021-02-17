#pragma once
#include "priv_types.hpp"
#include <unordered_map>
#include <atomic>
#include <thread>
#include <filesystem>
#include "bounded_buffer.hpp"
#include "node_display_tree.hpp"
namespace dt::df
{
    class GraphImpl final
    {
    public:
        GraphImpl();
        void registerNodeFactory(const NodeKey &key, const std::string &node_display_name, NodeFactory &&factory, NodeDeserializationFactory &&deser_factory);
        void createNode(const NodeKey &key, int preferred_x, int preferred_y, bool screen_space);
        void removeNode(const NodeId id);
        void addEdge(const VertexDesc from, const VertexDesc to);
        void removeEdge(const EdgeId id);
        VertexDesc findVertexById(const NodeId id) const;

        void renderNodes();
        void renderLinks();

        void save(const std::filesystem::path &file);
        void clearAndLoad(const std::filesystem::path &file);
        void clear();
        const NodeDisplayGraph &nodeDisplayNames() const;
        ~GraphImpl();

    private:
        void addNode(const NodePtr &node);
        const NodeFactory &getNodeFactory(const NodeKey &key) const;
        const NodeDeserializationFactory &getNodeDeserializationFactory(const NodeKey &key) const;
        VertexDesc addVertex(const VertexDesc node_desc, const int id, const int parent_id, VertexType type);
        void removeNodeSlots(const Slots &slots);
        void evaluationTask();
        void reevaluateSlot(SlotId slot);
        SlotPtr findSlotById(const SlotId) const;

    private:
        Graph graph_;
        IdGenerator link_id_counter_;
        IdGenerator vertex_id_counter_;
        std::unordered_map<NodeKey, NodeFactory> node_factories_;
        std::unordered_map<NodeKey, NodeDeserializationFactory> node_deser_factories_;
        NodeDisplayGraph node_display_names_;
        std::unordered_map<NodeId, NodePtr> nodes_;

        bool run_evaluation_;
        std::thread evaluation_thread_;
        bounded_buffer<SlotId> evaluation_queue_;
    };
} // namespace dt::df
