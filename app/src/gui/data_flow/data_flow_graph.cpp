#include "data_flow_graph.hpp"
#include <imnodes.h>

namespace gui::df
{
    DataFlowGraph::DataFlowGraph()
    {
    }

    void DataFlowGraph::addNode(const std::string &key)
    {
        if (auto it = node_factories_.find(key); it != node_factories_.end())
        {
            auto node = it->second();
            nodes_.emplace(node->id(), node);
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
            node.second->drawNode();
        }
    }

    void DataFlowGraph::drawLinks()
    {
        for (int i = 0; i < connections_.size(); ++i)
        {
            const std::pair<int, int> p = connections_[i];
            imnodes::Link(i, p.first, p.second);
        }
    }

    void DataFlowGraph::addPendingConnections()
    {
        int started_at_node_id;
        int started_at_attribute_id;
        int ended_at_node_id;
        int ended_at_attribute_id;
        if (imnodes::IsLinkCreated(&started_at_node_id, &started_at_attribute_id, &ended_at_node_id, &ended_at_attribute_id))
        {
            auto start_it = nodes_.find(started_at_node_id);
            auto end_it = nodes_.find(ended_at_node_id);
            if (start_it != nodes_.end() && end_it != nodes_.end())
            {
                auto start_slot = start_it->second->output_slots().find(started_at_attribute_id);
                auto end_slot = end_it->second->input_slots().find(ended_at_attribute_id);
                if (start_slot != start_it->second->output_slots().end() &&
                    end_slot != end_it->second->input_slots().end() &&
                    start_slot->second.canConnectWith(end_slot->second))
                {
                    connections_.emplace_back(std::make_pair(started_at_attribute_id, ended_at_attribute_id));
                }
            }
        }
    }

    void DataFlowGraph::deletePendingConnections()
    {
        int link_id;
        if (imnodes::IsLinkDestroyed(&link_id))
        {
            connections_.erase(connections_.begin() + link_id);
        }
    }

    DataFlowGraph::~DataFlowGraph()
    {
    }
} // namespace gui::df
