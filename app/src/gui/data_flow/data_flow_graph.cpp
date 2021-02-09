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
            VertexInfo node_info{VertexType::node};
            const auto vertex_id = boost::add_vertex(node_info, graph_);
            auto node = it->second(vertex_id);

            for (auto &slot : node->inputs())
            {
                VertexInfo info{VertexType::input};
                const auto slot_vertex_id = boost::add_vertex(info, graph_);
                slot.vertexDesc(slot_vertex_id);
                boost::add_edge(slot_vertex_id, vertex_id, EdgeProperty(link_id_counter_++), graph_);
            }
            for (auto &slot : node->outputs())
            {
                VertexInfo info{VertexType::output};
                const auto slot_vertex_id = boost::add_vertex(info, graph_);
                slot.vertexDesc(slot_vertex_id);
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
        for (const auto &node : nodes_)
        {
            const auto output_it = boost::out_edges(node.first, graph_);

            for (auto eit = output_it.first; eit != output_it.second; ++eit)
            {
                const auto output_link_it = boost::out_edges(boost::target(*eit, graph_), graph_);
                for (auto eeit = output_link_it.first; eeit != output_link_it.second; ++eeit)
                {
                    const auto edge_index = boost::get(boost::edge_index, graph_, *eeit);
                    imnodes::Link(edge_index, boost::source(*eeit, graph_), boost::target(*eeit, graph_));
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
            for (const auto &node : nodes_)
            {
                const auto output_it = boost::out_edges(node.first, graph_);

                for (auto eit = output_it.first; eit != output_it.second; ++eit)
                {
                    const auto output_link_it = boost::out_edges(boost::target(*eit, graph_), graph_);
                    for (auto eeit = output_link_it.first; eeit != output_link_it.second; ++eeit)
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

    DataFlowGraph::~DataFlowGraph()
    {
    }
} // namespace gui::df
