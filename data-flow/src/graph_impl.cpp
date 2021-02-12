#include "graph_impl.hpp"
#include <cassert>
#include <imnodes.h>
namespace dt::df
{

    GraphImpl::GraphImpl()
        : run_evaluation_{true},
          evaluation_thread_{std::bind(&GraphImpl::evaluationTask, this)},
          evaluation_queue_{200}
    {
    }
    void GraphImpl::registerNodeFactory(const NodeKey &key, NodeFactory &&factory)
    {
        node_factories_.emplace(key, std::forward<NodeFactory>(factory));
    }

    void GraphImpl::createNode(const NodeKey &key)
    {
        auto factory_fnc_it = node_factories_.find(key);
        if (factory_fnc_it == node_factories_.end())
            throw std::out_of_range("node factory not found");

        auto node = factory_fnc_it->second(vertex_id_counter_, vertex_id_counter_);
        nodes_.emplace(node->id(), node);

        for (auto &slot : node->inputs())
        {
            slot->connectEvaluation(std::bind(&GraphImpl::reevaluateSlot, this, std::placeholders::_1));
        }
        for (auto &slot : node->outputs())
        {
            slot->connectEvaluation(std::bind(&GraphImpl::reevaluateSlot, this, std::placeholders::_1));
        }

        const auto node_vertex = addVertex(0, node->id(), -1, VertexType::node);

        for (auto &slot : node->inputs())
            addVertex(node_vertex, slot->id(), node->id(), VertexType::input);

        for (auto &slot : node->outputs())
            addVertex(node_vertex, slot->id(), node->id(), VertexType::output);
    }

    void GraphImpl::removeNode(const NodeId id)
    {
        auto node_it = nodes_.find(id);
        if (node_it == nodes_.end())
            return;

        try
        {
            // remove node
            auto node_vertex = findVertexById(id);
            boost::clear_vertex(node_vertex, graph_);
        }
        catch (const std::out_of_range &)
        {
            //! \todo add logger
        }

        removeNodeSlots(node_it->second->inputs());
        removeNodeSlots(node_it->second->outputs());

        nodes_.erase(node_it);
    }

    VertexDesc GraphImpl::addVertex(const VertexDesc node_desc, const int id, const int parent_id, VertexType type)
    {
        VertexInfo info{id, parent_id, type};
        const auto vertex_desc = boost::add_vertex(std::move(info), graph_);
        if (type != VertexType::node)
        {
            EdgeInfo edge_info{link_id_counter_++, nullptr};
            if (type == VertexType::input)
                boost::add_edge(vertex_desc, node_desc, std::move(edge_info), graph_);
            else if (type == VertexType::output)
                boost::add_edge(node_desc, vertex_desc, std::move(edge_info), graph_);
        }
        return vertex_desc;
    }

    void GraphImpl::addEdge(const VertexDesc from, const VertexDesc to)
    {
        assert(("from needs to be an output", graph_[from].type == VertexType::output));
        assert(("to needs to be an input", graph_[to].type == VertexType::input));
        assert(("from parent isn't set", graph_[from].parent_id >= 0));
        assert(("to parent isn't set", graph_[to].parent_id >= 0));

        auto from_node = nodes_.find(graph_[from].parent_id);
        if (from_node == nodes_.end())
            assert("from parent not set correctly");

        auto to_node = nodes_.find(graph_[to].parent_id);
        if (to_node == nodes_.end())
            assert("to parent not set correctly");

        auto output_slot = from_node->second->outputs(graph_[from].id);
        if (!output_slot)
            assert("output is null. so id isn't correctly set");

        auto input_slot = to_node->second->inputs(graph_[to].id);
        if (!input_slot)
            assert("input is null. so id isn't correctly set");

        if (!output_slot->canConnect(input_slot.get()))
            return;

        const auto egde_prop = EdgeInfo{link_id_counter_++,
                                        std::make_shared<RefCon>(output_slot->subscribe([output_slot, input_slot]() {
                                            input_slot->accept(output_slot.get());
                                        }))};
        boost::add_edge(from, to, std::move(egde_prop), graph_);
    }

    void GraphImpl::removeEdge(const EdgeId id)
    {
        boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
        for (boost::tie(vi, vi_end) = boost::vertices(graph_); vi != vi_end; ++vi)
        {
            if (graph_[*vi].type == VertexType::output)
            {
                auto edges = boost::out_edges(*vi, graph_);
                for (auto eeit = edges.first; eeit != edges.second; ++eeit)
                {
                    const auto &edge_prop = boost::get(EdgeInfo_t(), graph_, *eeit);
                    if (edge_prop.id == id)
                    {
                        boost::remove_edge(*eeit, graph_);
                        break;
                    }
                }
            }
        }
    }

    VertexDesc GraphImpl::findVertexById(const NodeId id) const
    {
        boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
        for (boost::tie(vi, vi_end) = boost::vertices(graph_); vi != vi_end; ++vi)
        {
            if (graph_[*vi].id == id)
                return *vi;
        }
        throw std::out_of_range("vertex with id not found");
    }

    void GraphImpl::removeNodeSlots(const Slots &slots)
    {
        for (const auto &slot : slots)
        {
            boost::graph_traits<Graph>::vertex_iterator vi, vi_end, next;
            boost::tie(vi, vi_end) = boost::vertices(graph_);
            for (next = vi; vi != vi_end; vi = next)
            {
                ++next;
                if (graph_[*vi].id == slot->id())
                    boost::clear_vertex(*vi, graph_);
            }
        }
    }

    void GraphImpl::renderNodes()
    {
        for (auto &node : nodes_)
        {
            node.second->render();
        }
    }
    void GraphImpl::renderLinks()
    {
        boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
        for (boost::tie(vi, vi_end) = boost::vertices(graph_); vi != vi_end; ++vi)
        {
            if (graph_[*vi].type == VertexType::output)
            {
                const auto output_it = boost::out_edges(*vi, graph_);
                for (auto eeit = output_it.first; eeit != output_it.second; ++eeit)
                {
                    const auto &edge_prop = boost::get(EdgeInfo_t(), graph_, *eeit);
                    imnodes::Link(edge_prop.id, graph_[*vi].id, graph_[boost::target(*eeit, graph_)].id);
                }
            }
        }
    }

    void GraphImpl::evaluationTask()
    {
        while (run_evaluation_)
        {
            SlotPtr slot;
            evaluation_queue_.pop_back(&slot);
            if (slot)
            {
                slot->valueChanged();
            }
        }
    }

    void GraphImpl::reevaluateSlot(SlotPtr slot)
    {
        //! \todo we need to detect circles!
        evaluation_queue_.push_front(slot);
    }

    GraphImpl::~GraphImpl()
    {
        run_evaluation_ = false;
        if (evaluation_thread_.joinable())
            evaluation_thread_.join();
    }
} // namespace dt::df
