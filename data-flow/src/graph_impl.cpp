#include "graph_impl.hpp"
#include <cassert>
#include <imnodes.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include "nodes/core/json.hpp"
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
            EdgeInfo edge_info{link_id_counter_(), nullptr};
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

        auto connection = output_slot->subscribe(
            BaseSlot::ValueChangedSignal::slot_type(std::bind(&BaseSlot::accept, input_slot.get(), std::placeholders::_1))
                .track_foreign(input_slot));

        const EdgeInfo egde_prop{link_id_counter_(), std::make_shared<RefCon>(std::move(connection))};
        boost::add_edge(from, to, std::move(egde_prop), graph_);
        output_slot->connectEvent();
        input_slot->connectEvent();
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
                        const auto edge_source = boost::source(*eeit, graph_);
                        const auto edge_target = boost::target(*eeit, graph_);
                        auto slot_source = findSlotById(graph_[edge_source].id);
                        slot_source->disconnectEvent();
                        auto slot_target = findSlotById(graph_[edge_target].id);
                        slot_target->disconnectEvent();
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
                {
                    if (graph_[*vi].type == VertexType::input)
                    {
                        const auto in_edges = boost::in_edges(*vi, graph_);
                        for (auto it = in_edges.first; it != in_edges.second; it++)
                        {
                            auto slot_in = findSlotById(graph_[boost::source(*it, graph_)].id);
                            slot_in->disconnectEvent();
                        }
                    }
                    else if (graph_[*vi].type == VertexType::output)
                    {
                        const auto out_edges = boost::out_edges(*vi, graph_);
                        for (auto it = out_edges.first; it != out_edges.second; it++)
                        {
                            auto slot_in = findSlotById(graph_[boost::target(*it, graph_)].id);
                            slot_in->disconnectEvent();
                        }
                    }
                    boost::clear_vertex(*vi, graph_);
                }
            }
        }
    }

    SlotPtr GraphImpl::findSlotById(const SlotId id) const
    {
        try
        {
            const auto slot_desc = findVertexById(id);
            assert(("id is not an slot id", graph_[slot_desc].type != VertexType::node));
            assert(("parent isn't set", graph_[slot_desc].parent_id >= 0));
            if (auto nit = nodes_.find(graph_[slot_desc].parent_id); nit != nodes_.end())
            {
                if (graph_[slot_desc].type == VertexType::input)
                    return nit->second->inputs(id);
                else if (graph_[slot_desc].type == VertexType::output)
                    return nit->second->outputs(id);
            }
        }
        catch (...)
        {
            //! \todo log
        }
        return nullptr;
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
            SlotId slot_id;
            evaluation_queue_.pop_back(&slot_id);
            if (slot_id < 0)
                continue;

            auto slot = findSlotById(slot_id);
            if (slot)
                slot->valueChanged();
        }
    }

    void GraphImpl::reevaluateSlot(SlotId slot)
    {
        //! \todo we need to detect circles!
        evaluation_queue_.push_front(slot);
    }

    void GraphImpl::save(const std::filesystem::path &file)
    {
        using json = nlohmann::json;

        json all_json;
        json nodes_json = json::array();
        for (const auto &node : nodes_)
        {
            nodes_json.push_back(*node.second);
        }
        all_json["nodes"] = std::move(nodes_json);

        json edges_json = json::array();
        boost::graph_traits<Graph>::edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::edges(graph_); ei != ei_end; ++ei)
        {
            const auto &source_info = graph_[boost::source(*ei, graph_)];
            const auto &target_info = graph_[boost::target(*ei, graph_)];
            // only save outside linkage
            if (source_info.type != VertexType::node && target_info.type != VertexType::node)
                edges_json.emplace_back(json{graph_[boost::source(*ei, graph_)].id, graph_[boost::target(*ei, graph_)].id});
        }
        all_json["links"] = std::move(edges_json);

        std::ofstream o(file);
        o << all_json << std::endl;
    }

    void GraphImpl::clearAndLoad(const std::filesystem::path &file)
    {
        clear();
    }

    void GraphImpl::clear()
    {
        graph_.clear();
        nodes_.clear();
    }

    GraphImpl::~GraphImpl()
    {
        evaluation_queue_.push_front(-1); // wake up
        run_evaluation_ = false;
        if (evaluation_thread_.joinable())
            evaluation_thread_.join();
    }
} // namespace dt::df
