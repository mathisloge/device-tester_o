#include "data_flow_graph.hpp"
#include <imnodes.h>

namespace gui::df
{
    DataFlowGraph::DataFlowGraph()
        : link_id_counter_(0)
    {
    }

    void DataFlowGraph::addNode(const std::string &key)
    {
        if (auto it = node_factories_.find(key); it != node_factories_.end())
        {
            VertexInfo node_info{vertex_id_counter_++, VertexType::node};
            const auto vertex_id = boost::add_vertex(node_info, graph_);
            auto node = it->second(node_info.id);

            for (auto &slot : node->inputs())
            {
                VertexInfo info{vertex_id_counter_++, VertexType::input};
                const auto slot_vertex_id = boost::add_vertex(info, graph_);
                slot.id(info.id);
                boost::add_edge(slot_vertex_id, vertex_id, EdgeProperty(link_id_counter_++), graph_);
            }
            for (auto &slot : node->outputs())
            {
                VertexInfo info{vertex_id_counter_++, VertexType::output};
                const auto slot_vertex_id = boost::add_vertex(info, graph_);

                slot.id(info.id);
                boost::add_edge(vertex_id, slot_vertex_id, EdgeProperty(link_id_counter_++), graph_);
            }

            nodes_.emplace(vertex_id, node);
        }
    }

    void DataFlowGraph::registerNodeFactory(const std::string &key, NodeFactoryFnc &&factory)
    {
        node_factories_.emplace(key, std::forward<NodeFactoryFnc>(factory));
    }

    void DataFlowGraph::drawNodes()
    {
        for (auto &node : nodes_)
        {
            node.second->draw(graph_);
        }
    }

    void DataFlowGraph::drawLinks()
    {
        boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
        for (boost::tie(vi, vi_end) = boost::vertices(graph_); vi != vi_end; ++vi)
        {
            if (graph_[*vi].type == VertexType::output)
            {
                const auto output_it = boost::out_edges(*vi, graph_);
                for (auto eeit = output_it.first; eeit != output_it.second; ++eeit)
                {
                    const auto edge_index = boost::get(boost::edge_index, graph_, *eeit);
                    imnodes::Link(edge_index, *vi, boost::target(*eeit, graph_));
                }
            }
        }
    }

    void DataFlowGraph::addPendingConnections()
    {
        int started_at_attribute_id;
        int ended_at_attribute_id;
        if (imnodes::IsLinkCreated(&started_at_attribute_id, &ended_at_attribute_id))
        {
            boost::add_edge(started_at_attribute_id, ended_at_attribute_id, EdgeProperty(link_id_counter_++), graph_);
        }
    }

    void DataFlowGraph::deletePendingConnections()
    {
        int link_id;
        if (imnodes::IsLinkDestroyed(&link_id))
        {
            boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
            for (boost::tie(vi, vi_end) = boost::vertices(graph_); vi != vi_end; ++vi)
            {
                if (graph_[*vi].type == VertexType::output)
                {
                    auto edges = boost::out_edges(*vi, graph_);
                    for (auto eeit = edges.first; eeit != edges.second; ++eeit)
                    {
                        const auto edge_index = boost::get(boost::edge_index, graph_, *eeit);
                        if (edge_index == link_id)
                        {
                            boost::remove_edge(*eeit, graph_);
                            break;
                        }
                    }
                }
            }
        }
    }

    void DataFlowGraph::removeNode(const int vertex_id)
    {
        auto node_it = nodes_.find(vertex_id);
        if (node_it == nodes_.end())
            return;

        try
        {
            // remove node
            auto node_vertex = findVertex(vertex_id);
            boost::clear_vertex(node_vertex, graph_);
        }
        catch (const std::out_of_range &ex)
        {
            //! \todo add logger
        }

        deleteNodeSlots(node_it->second->inputs());
        deleteNodeSlots(node_it->second->outputs());

        nodes_.erase(node_it);
    }

    VertexDesc DataFlowGraph::findVertex(const int vertex_id)
    {
        boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
        for (boost::tie(vi, vi_end) = boost::vertices(graph_); vi != vi_end; ++vi)
        {
            if (graph_[*vi].id == vertex_id)
                return *vi;
        }
        throw std::out_of_range("vertex id not found");
    }

    void DataFlowGraph::deleteNodeSlots(const UiNode::Slots &slots)
    {
        for (const auto &slot : slots)
        {
            boost::graph_traits<Graph>::vertex_iterator vi, vi_end, next;
            boost::tie(vi, vi_end) = boost::vertices(graph_);
            for (next = vi; vi != vi_end; vi = next)
            {
                ++next;
                if (graph_[*vi].id == slot.id())
                    boost::clear_vertex(*vi, graph_);
            }
        }
    }

    DataFlowGraph::~DataFlowGraph()
    {
    }
} // namespace gui::df
