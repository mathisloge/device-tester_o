#include "graph_impl.hpp"
#include <cassert>
namespace dt::df
{
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

        const auto egde_prop = EdgeInfo{link_id_counter_++, output_slot->subscribe([output_slot, input_slot]() {
                                            input_slot->accept(output_slot.get());
                                        })};
        boost::add_edge(from, to, std::move(egde_prop), graph_);
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
} // namespace dt::df
